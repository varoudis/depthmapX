#include "salalib/vgamodules/visualglobal.h"

#include "genlib/stringutils.h"

bool VGAVisualGlobal::run(Communicator *comm, const Options &options, PointMap &map, bool simple_version) {
    time_t atime = 0;
    if (comm) {
        qtimer(atime, 0);
        comm->CommPostMessage(Communicator::NUM_RECORDS, map.getFilledPointCount());
    }
    AttributeTable &attributes = map.getAttributeTable();

    int entropy_col, rel_entropy_col, integ_dv_col, integ_pv_col, integ_tk_col, depth_col, count_col;
    std::string radius_text;
    if (options.radius != -1) {
        radius_text = std::string(" R") + dXstring::formatString(int(options.radius), "%d");
    }

    // n.b. these must be entered in alphabetical order to preserve col indexing:
    // dX simple version test // TV
#ifndef _COMPILE_dX_SIMPLE_VERSION
    if (!simple_version) {
        std::string entropy_col_text = std::string("Visual Entropy") + radius_text;
        entropy_col = attributes.insertColumn(entropy_col_text.c_str());
    }
#endif

    std::string integ_dv_col_text = std::string("Visual Integration [HH]") + radius_text;
    integ_dv_col = attributes.insertColumn(integ_dv_col_text.c_str());

#ifndef _COMPILE_dX_SIMPLE_VERSION
    if (!simple_version) {
        std::string integ_pv_col_text = std::string("Visual Integration [P-value]") + radius_text;
        integ_pv_col = attributes.insertColumn(integ_pv_col_text.c_str());
        std::string integ_tk_col_text = std::string("Visual Integration [Tekl]") + radius_text;
        integ_tk_col = attributes.insertColumn(integ_tk_col_text.c_str());
        std::string depth_col_text = std::string("Visual Mean Depth") + radius_text;
        depth_col = attributes.insertColumn(depth_col_text.c_str());
        std::string count_col_text = std::string("Visual Node Count") + radius_text;
        count_col = attributes.insertColumn(count_col_text.c_str());
        std::string rel_entropy_col_text = std::string("Visual Relativised Entropy") + radius_text;
        rel_entropy_col = attributes.insertColumn(rel_entropy_col_text.c_str());
    }
#endif

    int count = 0;

    for (int i = 0; i < map.getCols(); i++) {

        for (int j = 0; j < map.getRows(); j++) {

            PixelRef curs = PixelRef(i, j);

            if (map.getPoint(curs).filled()) {

                if ((map.getPoint(curs).contextfilled() && !curs.iseven()) || (options.gates_only)) {
                    count++;
                    continue;
                }

                for (int ii = 0; ii < map.getCols(); ii++) {
                    for (int jj = 0; jj < map.getRows(); jj++) {
                        map.getPoint(static_cast<PixelRef>(ii * map.getRows() + jj)).m_misc = 0;
                        map.getPoint(static_cast<PixelRef>(ii * map.getRows() + jj)).m_extent = PixelRef(ii, jj);
                    }
                }

                int total_depth = 0;
                int total_nodes = 0;

                pvecint distribution;
                prefvec<PixelRefVector> search_tree;
                search_tree.push_back(PixelRefVector());
                search_tree.tail().push_back(curs);

                int level = 0;
                while (search_tree[level].size()) {
                    search_tree.push_back(PixelRefVector());
                    distribution.push_back(0);
                    for (size_t n = search_tree[level].size() - 1; n != paftl::npos; n--) {
                        Point &p = map.getPoint(search_tree[level][n]);
                        if (p.filled() && p.m_misc != ~0) {
                            total_depth += level;
                            total_nodes += 1;
                            distribution.tail() += 1;
                            if ((int)options.radius == -1 ||
                                level < (int)options.radius &&
                                    (!p.contextfilled() || search_tree[level][n].iseven())) {
                                p.getNode().extractUnseen(search_tree[level + 1], &map, p.m_misc);
                                p.m_misc = ~0;
                                if (!p.getMergePixel().empty()) {
                                    Point &p2 = map.getPoint(p.getMergePixel());
                                    if (p2.m_misc != ~0) {
                                        p2.getNode().extractUnseen(search_tree[level + 1], &map,
                                                                 p2.m_misc); // did say p.misc
                                        p2.m_misc = ~0;
                                    }
                                }
                            } else {
                                p.m_misc = ~0;
                            }
                        }
                        search_tree[level].pop_back();
                    }
                    level++;
                }
                int row = attributes.getRowid(curs);
                // only set to single float precision after divide
                // note -- total_nodes includes this one -- mean depth as per p.108 Social Logic of Space
                if (!simple_version) {
                    attributes.setValue(row, count_col, float(total_nodes)); // note: total nodes includes this one
                }
                // ERROR !!!!!!
                if (total_nodes > 1) {
                    double mean_depth = double(total_depth) / double(total_nodes - 1);
                    if (!simple_version) {
                        attributes.setValue(row, depth_col, float(mean_depth));
                    }
                    // total nodes > 2 to avoid divide by 0 (was > 3)
                    if (total_nodes > 2 && mean_depth > 1.0) {
                        double ra = 2.0 * (mean_depth - 1.0) / double(total_nodes - 2);
                        // d-value / p-values from Depthmap 4 manual, note: node_count includes this one
                        double rra_d = ra / dvalue(total_nodes);
                        double rra_p = ra / pvalue(total_nodes);
                        double integ_tk = teklinteg(total_nodes, total_depth);
                        attributes.setValue(row, integ_dv_col, float(1.0 / rra_d));
                        if (!simple_version) {
                            attributes.setValue(row, integ_pv_col, float(1.0 / rra_p));
                        }
                        if (total_depth - total_nodes + 1 > 1) {
                            if (!simple_version) {
                                attributes.setValue(row, integ_tk_col, float(integ_tk));
                            }
                        } else {
                            if (!simple_version) {
                                attributes.setValue(row, integ_tk_col, -1.0f);
                            }
                        }
                    } else {
                        attributes.setValue(row, integ_dv_col, (float)-1);
                        if (!simple_version) {
                            attributes.setValue(row, integ_pv_col, (float)-1);
                            attributes.setValue(row, integ_tk_col, (float)-1);
                        }
                    }
                    double entropy = 0.0, rel_entropy = 0.0, factorial = 1.0;
                    // n.b., this distribution contains the root node itself in distribution[0]
                    // -> chopped from entropy to avoid divide by zero if only one node
                    for (size_t k = 1; k < distribution.size(); k++) {
                        if (distribution[k] > 0) {
                            double prob = double(distribution[k]) / double(total_nodes - 1);
                            entropy -= prob * log2(prob);
                            // Formula from Turner 2001, "Depthmap"
                            factorial *= double(k + 1);
                            double q = (pow(mean_depth, double(k)) / double(factorial)) * exp(-mean_depth);
                            rel_entropy += (float)prob * log2(prob / q);
                        }
                    }
                    if (!simple_version) {
                        attributes.setValue(row, entropy_col, float(entropy));
                        attributes.setValue(row, rel_entropy_col, float(rel_entropy));
                    }
                } else {
                    if (!simple_version) {
                        attributes.setValue(row, depth_col, (float)-1);
                        attributes.setValue(row, entropy_col, (float)-1);
                        attributes.setValue(row, rel_entropy_col, (float)-1);
                    }
                }
            }

            count++; // <- increment count
            if (comm) {
                if (qtimer(atime, 500)) {
                    if (comm->IsCancelled()) {
                        throw Communicator::CancelledException();
                    }
                    comm->CommPostMessage(Communicator::CURRENT_RECORD, count);
                }
            }
        }
    }
    map.setDisplayedAttribute(integ_dv_col);

    return true;
}

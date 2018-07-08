#include "salalib/tidylines.h"
#include "salalib/tolerances.h"

// helper -- a little class to tidy up a set of lines

void TidyLines::tidy(std::vector<Line>& lines, const QtRegion& region)
{
   m_region = region;
   double maxdim = std::max(m_region.width(),m_region.height());

   // simple first pass -- remove very short lines
   lines.erase(
               std::remove_if(lines.begin(), lines.end(),
                              [maxdim](const Line& line)
   {return line.length() < maxdim * TOLERANCE_B;}), lines.end());

   // now load up m_lines...
   initLines(lines.size(),m_region.bottom_left,m_region.top_right);
   for (auto& line: lines) {
      addLine(line);
   }
   sortPixelLines();

   std::vector<int> removelist;
   for (size_t i = 0; i < lines.size(); i++) {
      // n.b., as m_lines have just been made, note that what's in m_lines matches whats in lines
      // we will use this later!
      m_test++;
      m_lines[i].test = m_test;
      PixelRefVector list = pixelateLine( m_lines[i].line );
      for (size_t a = 0; a < list.size(); a++) {
         for (size_t b = 0; b < m_pixel_lines[ list[a].x ][ list[a].y ].size(); b++) {
            int j = m_pixel_lines[ list[a].x ][ list[a].y ][b];
            if (m_lines[j].test != m_test && j > (int)i && intersect_region(lines[i],lines[j],TOLERANCE_B * maxdim)) {
               m_lines[j].test = m_test;
               int axis_i = (lines[i].width() >= lines[i].height()) ? XAXIS : YAXIS;
               int axis_j = (lines[j].width() >= lines[j].height()) ? XAXIS : YAXIS;
               int axis_reverse = (axis_i == XAXIS) ? YAXIS : XAXIS;
               if (axis_i == axis_j && fabs(lines[i].grad(axis_reverse) - lines[j].grad(axis_reverse)) < TOLERANCE_A
                                    && fabs(lines[i].constant(axis_reverse) - lines[j].constant(axis_reverse)) < (TOLERANCE_B * maxdim)) {
                  // check for overlap and merge
                  int parity = (axis_i == XAXIS) ? 1 : lines[i].sign();
                  if ((lines[i].start()[axis_i] * parity + TOLERANCE_B * maxdim) > (lines[j].start()[axis_j] * parity) &&
                      (lines[i].start()[axis_i] * parity) < (lines[j].end()[axis_j] * parity + TOLERANCE_B * maxdim)) {
                     int end = ((lines[i].end()[axis_i] * parity) > (lines[j].end()[axis_j] * parity)) ? i : j;
                     lines[j].bx() = lines[end].bx();
                     lines[j].by() = lines[end].by();
                     removelist.push_back(i);
                     continue; // <- don't do this any more, we've zapped it and replaced it with the later line
                  }
                  if ((lines[j].start()[axis_j] * parity + TOLERANCE_B * maxdim) > (lines[i].start()[axis_i] * parity) &&
                      (lines[j].start()[axis_j] * parity) < (lines[i].end()[axis_i]  * parity + TOLERANCE_B * maxdim)) {
                     int end = ((lines[i].end()[axis_i] * parity) > (lines[j].end()[axis_j] * parity)) ? i : j;
                     lines[j].ax() = lines[i].ax();
                     lines[j].ay() = lines[i].ay();
                     lines[j].bx() = lines[end].bx();
                     lines[j].by() = lines[end].by();
                     removelist.push_back(i);
                     continue; // <- don't do this any more, we've zapped it and replaced it with the later line
                  }
               }
            }
         }
      }
   }

   // comes out sorted, remove duplicates just in case
   removelist.erase(std::unique(removelist.begin(), removelist.end()), removelist.end());

   for(auto iter = removelist.rbegin(); iter != removelist.rend(); ++iter)
       lines.erase(lines.begin() + *iter);
   removelist.clear();  // always clear this list, it's reused
}

void TidyLines::quicktidy(std::map<int,Line>& lines, const QtRegion& region)
{
   m_region = region;

   double avglen = 0.0;

   for (auto line: lines) {
      avglen += line.second.length();
   }
   avglen /= lines.size();

   double tolerance = avglen * 10e-6;

   auto iter = lines.begin(), end = lines.end();
   for(; iter != end; ) {
       if (iter->second.length() < tolerance) {
           iter = lines.erase(iter);
       } else {
           ++iter;
       }
   }

   // now load up m_lines...
   initLines(lines.size(),m_region.bottom_left,m_region.top_right);
   for (auto line: lines) {
      addLine(line.second);
   }
   sortPixelLines();

   // and chop duplicate lines:
   std::vector<int> removelist;
   int i = -1;
   for (auto line: lines) {
      i++;
      PixelRef start = pixelate(line.second.start());
      for (size_t j = 0; j < m_pixel_lines[start.x][start.y].size(); j++) {
         int k = m_pixel_lines[start.x][start.y][j];
         if (k > (int)i && approxeq(m_lines[i].line.start(),m_lines[k].line.start(),tolerance)) {
            if (approxeq(m_lines[i].line.end(),m_lines[k].line.end(),tolerance)) {
               removelist.push_back(line.first);
               break;
            }
         }
      }
   }
   for(int remove: removelist) {
       lines.erase(remove);
   }
   removelist.clear(); // always clear this list, it's reused}
}

// Copyright (C) 2011-2012, Tasos Varoudis

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QtCore/QFile>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <stdio.h>

#include "mainwindow.h"

#include "MakeLayerDlg.h"
#include "OptionsDlg.h"
#include "AxialAnalysisOptionsDlg.h"
#include "SegmentAnalysisDlg.h"
#include "GridDialog.h"
#include "MakeOptionsDlg.h"
#include "EditConnectionsDlg.h"
#include "ColourScaleDlg.h"
#include "FewestLineOptionsDlg.h"
#include "PromptReplace.h"
#include "FilePropertiesDlg.h"
#include "InsertColumnDlg.h"
#include "PushDialog.h"
#include "AgentAnalysisDlg.h"
#include "NewLayerDlg.h"
#include "AttributeChooserDlg.h"
#include "LayerChooserDlg.h"
#include "RenameObjectDlg.h"
#include "ColumnPropertiesDlg.h"
#include "IsovistPathDlg.h"
#include "ConvertShapesDlg.h"
#include "TopoMetDlg.h"
#include "AttributeSummary.h"
#include "depthmapView.h"
#include "viewhelpers.h"
#include <QMetaType>

#ifdef _WIN32
#include <windows.h>
#endif
#include "compatibilitydefines.h"
#include "salalib/importutils.h"

QT_BEGIN_NAMESPACE
Q_DECLARE_METATYPE(std::string)

QGraphDoc::QGraphDoc(const QString &author, const QString &organisation)
{
   m_opened_name = "";
   m_flag_lock = false;
   for (int i = 0; i < VIEW_TYPES; i++) 
   {
      m_redraw_flag[i] = REDRAW_DONE;
      m_remenu_flag[i] = false;
	  m_view[i] = NULL;
   }
   m_communicator = NULL;

   m_make_algorithm = 0;   // algorithm to make graph
   m_make_maxdist = -1.0;  // maximum distance you can see (set to -1.0 for infinite)

   m_meta_graph = new MetaGraph;

   modifiedFlag = false;

   modify_prog = false;
   m_num_steps = 0;
   m_record = 0;
   m_step = 0;
   m_num_records = 0;

   std::string date = ViewHelpers::getCurrentDate();
   QString version = QString(TITLE_BASE);

   m_meta_graph->setProperties(author.toStdString(),organisation.toStdString(),date,version.toStdString());

    qRegisterMetaType< std::string >();
    connect(&m_thread, &RenderThread::runtimeExceptionThrown, this, &QGraphDoc::exceptionThrownInRenderThread);
}
void QGraphDoc::exceptionThrownInRenderThread(int type, std::string message) {
    if(type == depthmapX::PointMapExceptionType::NO_ISOVIST_ANALYSIS) {
        std::stringstream message;
        message << "This operation requires isovist analysis. To run it go to: ";
        message << "Tools -> Visibility -> Run Visibility Graph Analysis... ";
        message << "and select \"Calculate isovist properties\"";
        message << flush;
        QMessageBox::warning(this, tr("Warning"), tr(message.str().c_str()),
                             QMessageBox::Ok, QMessageBox::Ok);
    }
}

bool QGraphDoc::SetRedrawFlag(int viewtype, int flag, int reason, QWidget *originator) // (almost) thread safe
{

    if(viewtype == VIEW_ALL && flag != REDRAW_DONE)
    {
        ((MainWindow *) m_mainFrame)->updateGLWindows(true, flag == REDRAW_TOTAL);
    }
    if(viewtype == VIEW_MAP && flag == REDRAW_TOTAL)
    {
        ((MainWindow *) m_mainFrame)->updateGLWindows(false, true);
    }

   if (!m_flag_lock) {
      m_flag_lock = true;
      if (viewtype) {
         // it's the view calling itself
         if (m_redraw_flag[viewtype] < flag) 
		 {
            m_redraw_flag[viewtype] = flag;
            QApplication::postEvent(m_view[viewtype], new QEvent(QEvent::FocusIn));

         }
         if (flag == REDRAW_DONE) {
            m_redraw_flag[viewtype] = flag;
         }
      }
      else {
         for (int i = 1; i < VIEW_TYPES; i++) {
            if (m_view[i] && m_redraw_flag[i] < flag) 
		    {
               m_redraw_flag[i] = flag;
               QApplication::postEvent(m_view[i], new QEvent(QEvent::FocusIn));
            }
         }
      }
      m_flag_lock = false;
      return true;
   }
   return false;
}

void QGraphDoc::UpdateMainframestatus()
{
    QString s1, s2, s3;
    if (!m_meta_graph->viewingNone()) {
       int n = 0;
       int state = m_meta_graph->getState();
       // showing the axial graph
       if ((state & MetaGraph::SHAPEGRAPHS) && m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL) {
          n = (int) m_meta_graph->getDisplayedShapeGraph().getShapeCount();
       }
       else if ((state & MetaGraph::DATAMAPS) && m_meta_graph->getViewClass() & MetaGraph::VIEWDATA) {
          n = (int) m_meta_graph->getDisplayedDataMap().getShapeCount();
       }
       // either showing or constructing the VGA graph
       else if ((state & MetaGraph::POINTMAPS) && m_meta_graph->getViewClass() & MetaGraph::VIEWVGA) {
          n = (int) m_meta_graph->getDisplayedPointMap().getPointCount();
       }
       if (n > 0) {
          s1 = QString("%1   ").arg(n);
       }
       QtRegion r = m_meta_graph->getBoundingBox();
       s2 = QString("%1,  %2   ").arg(r.width()).arg(r.height());
       s3 = QString("%1,  %2   ").arg(m_position.x).arg(m_position.y);
       ((MainWindow *)m_mainFrame)->UpdateStatus(s1, s2, s3);
    }
}

void QGraphDoc::SetUpdateFlag(int type, bool modified)
{
   switch (type) {
   case NEW_FILE:
        QApplication::postEvent((QObject*)m_mainFrame, new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void*)this, CONTROLS_LOADALL));
      break;
   case NEW_DATA:
        QApplication::postEvent((QObject*)m_mainFrame, new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void*)this, CONTROLS_LOADATTRIBUTES));
      break;
   case NEW_TABLE:
        QApplication::postEvent((QObject*)m_mainFrame, new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void*)this, CONTROLS_LOADGRAPH));
      break;
   case DELETED_TABLE:
        QApplication::postEvent((QObject*)m_mainFrame, new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void*)this, CONTROLS_RELOADGRAPH));
      break;
   }
   SetRemenuFlag(VIEW_ALL,true);     // Tell the views to update their menus
   if (modified) {
	   modifiedFlag = true;
   }
}

/////////////////////////////////////////////////////////////////////////////
// QGraphDoc commands

// New layer and delete layer

void QGraphDoc::OnLayerNew() 
{
   CNewLayerDlg dlg;
   if (QDialog::Accepted == dlg.exec()) {
      // insert a new layer of the correct type
      // for now, 0 = axial map, and 1 = data map
      ShapeMap *map;
      if (dlg.m_layer_type == 0) {
          int ref = m_meta_graph->addShapeMap(dlg.m_name.toStdString());
         map = &(m_meta_graph->getDataMaps().getMap(ref));
      }
      else if (dlg.m_layer_type == 1) {
         int ref = m_meta_graph->addShapeGraph(dlg.m_name.toStdString(),ShapeMap::CONVEXMAP);
         map = &(m_meta_graph->getShapeGraphs().getMap(ref));
      }
      else if (dlg.m_layer_type == 2) {
         int ref = m_meta_graph->addShapeGraph(dlg.m_name.toStdString(),ShapeMap::AXIALMAP);
         map = &(m_meta_graph->getShapeGraphs().getMap(ref));
      }
      else if (dlg.m_layer_type == 3) {
         int ref = m_meta_graph->addShapeGraph(dlg.m_name.toStdString(),ShapeMap::PESHMAP);
         map = &(m_meta_graph->getShapeGraphs().getMap(ref));
      }

      QtRegion r = m_meta_graph->getBoundingBox();
      if (r.atZero()) {
         r = QtRegion(Point2f(-50.0,-50.0),Point2f(50.0,50.0));
      }
      map->init(0,r);
      map->setEditable(true);

      SetUpdateFlag(NEW_TABLE);
      SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
   }
}

void QGraphDoc::OnLayerDelete() 
{
   // Delete the currently displayed map
    if (QMessageBox::Yes == QMessageBox::question(this, tr("depthmapX"), tr("Are you sure you want to delete this map?\nThis action cannot be undone"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No))
   {
      m_meta_graph->removeDisplayedMap();
      SetUpdateFlag(DELETED_TABLE);
      SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
   }
}

// Note: import and export are now on the Layer menu, not the file menu

// Import file types: .cat, .dxf, .ntf
// Txt files for points and lines (shapes)
void QGraphDoc::CreateWaitDialog(const QString& description, QWidget *pr)
{
	modify_prog = false;
	m_waitdlg = new QProgressDialog();
	connect(m_waitdlg, SIGNAL(canceled()), this, SLOT(cancel_wait()));
	m_waitdlg->setCancelButtonText(tr("&Cancel"));
	m_waitdlg->setWindowTitle(description);
	m_waitdlg->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog);
	m_waitdlg->show();
	Tid_progress = startTimer(50);
	m_timer.start();
}

void QGraphDoc::DestroyWaitDialog()
{
	modify_prog = true;
	killTimer(Tid_progress);
	QApplication::postEvent((QObject*)m_waitdlg, new QEvent(QEvent::Close));
}

void QGraphDoc::cancel_wait()
{
	if (m_communicator) {
		m_waitdlg->setLabelText(QString(" attempting to cancel"));
		if (m_communicator->IsCancelled()) {
		}
		else {
			m_communicator->Cancel();
		}
	// Don't cancel --- cancel should be handled by the thread! 
	}
}

void QGraphDoc::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == Tid_progress)
	{
		if(modify_prog) {
			QApplication::postEvent((QObject*)m_waitdlg, new QEvent(QEvent::Close));
			killTimer(Tid_progress);
			return;
		}

		QString lstr = QString("Step %1 of %2").arg(m_step).arg(m_num_steps);
		m_waitdlg->setValue(m_record);

		double percent = (100.0 * double(m_record)) / double(m_num_records);
		QString str;
		int timeleft = 1 + int((100.0 / percent - 1.0) * double(m_timer.elapsed()/1000));
        if (percent > 0.5) {
			if (timeleft >= 3600) {
				str = QString(" : Estimated %1 hours %2 minutes remaining").arg(timeleft / 3600).arg((timeleft / 60) % 60 );
            }
            else if (timeleft >= 60) {
				str = QString(" : Estimated %1 minutes %2 seconds remaining").arg((timeleft / 60) % 60).arg(timeleft % 60 );
			}
            else {
				str = QString(" : Estimated %1 seconds remaining").arg(timeleft % 60);
			}
        }
		lstr += str;
		m_waitdlg->setLabelText(lstr);
	}
}

void QGraphDoc::ProcPostMessage(int m, int x, int y)
{
	switch (m) {
	case Communicator::NUM_STEPS:
		m_num_steps = x;
		break;
	case Communicator::NUM_RECORDS:
		if(!modify_prog) m_waitdlg->setRange(0, x);
		 m_record = 0;
		 m_num_records = x;
		 break;
	case Communicator::CURRENT_STEP:
		 m_step = x;
         break;
    case Communicator::CURRENT_RECORD:
		 m_record = x;
		 if (m_record > m_num_records) m_record = m_num_records;
         break;
    }
}

void QGraphDoc::OnVGALinksFileImport()
{
    if (m_communicator) {
        return; // Locked
    }

    // change the view before loading the file to make the changes apparent
    if(m_view[VIEW_MAP])
        ((QDepthmapView*)m_view[VIEW_MAP])->m_showlinks = true;
    SetRedrawFlag(VIEW_MAP,REDRAW_POINTS, NEW_DEPTHMAPVIEW_SETUP);

    QString template_string;
    template_string += "Text files (*.txt *.csv)\n";
    template_string += "All files (*.*)";

    QFileDialog::Options options = 0;
    QString selectedFilter;
    QString infile = QFileDialog::getOpenFileName(
                                0, tr("Import Links File"),
                                "",
                                template_string,
                                &selectedFilter,
                                options);

    if (!infile.size())
    {
        // no file selected
        return;
    }

    std::string fileName = infile.toStdString();

    ifstream fileStream(fileName);
    if (fileStream.fail())
    {
      QMessageBox::warning(this, tr("Warning"), tr("Unable to read text file.\nPlease check that another program is not using it."),
                           QMessageBox::Ok, QMessageBox::Ok);
      return;
    }
    else
    {
        try
        {
            PointMap& currentMap = m_meta_graph->getDisplayedPointMap();
            vector<PixelRefPair> newLinks = depthmapX::pixelateMergeLines(
                        EntityParsing::parseLines(fileStream, '\t'), currentMap);
            depthmapX::mergePixelPairs(newLinks, currentMap);
            SetRedrawFlag(VIEW_MAP,REDRAW_POINTS, NEW_DEPTHMAPVIEW_SETUP);
        }
        catch (EntityParsing::EntityParseException& e)
        {
            std::stringstream message;
            message << "Unable to parse text file\n\n";
            message << fileName;
            message << "\n\n Error: ";
            message << e.what();
            message << flush;
            QMessageBox::warning(this, tr("Warning"), tr(message.str().c_str()),
                                                   QMessageBox::Ok, QMessageBox::Ok);
        }
        catch (depthmapX::InvalidLinkException& e)
        {
            std::stringstream message;
            message << "Unable to import links\n\n";
            message << fileName;
            message << "\n\n Error: ";
            message << e.what();
            message << flush;
            QMessageBox::warning(this, tr("Warning"), tr(message.str().c_str()),
                                                   QMessageBox::Ok, QMessageBox::Ok);
        }
    }
}

void QGraphDoc::OnFileImport()
{
   if (m_communicator) {
      return; // Locked
   }

   QString template_string;
   template_string += "All formats(*.dxf *.ntf *.gml *.cat *.rt1 *.mif *.txt *.csv)\n";
   template_string += "Drawing files (*.dxf *.ntf *.gml *.cat *.rt1)\nMapInfo map (*.mif)\nText files (*.txt *.csv)\n";
   
   template_string += "All files (*.*)";

    QFileDialog::Options options = 0;
    QString selectedFilter;
    QStringList infiles = QFileDialog::getOpenFileNames(
                                0, tr("Import Files"),
                                "",
                                template_string,
                                &selectedFilter,
                                options);

   if (!infiles.size()) {
      return;
   }

    // this is placed here as a proxy to be queried later so that we can find
    // if there was something else in the graph after importing. If there was
    // then the view should not be reset, if there wasn't then the view can
    // be reset to point to the newly imported objects
    bool graphHadNullBoundsBeforeImport = m_meta_graph->getBoundingBox().atZero();

   QFilePath filepath(infiles[0]);
   QString ext = filepath.m_ext;
   if (ext == tr("CAT") || ext == tr("DXF") || ext == tr("NTF") || ext == tr("RT1") || ext == tr("MIF") || ext == tr("GML") || ext == tr("")) {
      for (int i = 1; i < infiles.size(); i++) {
         QFilePath filepath(infiles[i]);
         if (filepath.m_ext != ext || !(filepath.m_ext == tr("RT1") || filepath.m_ext == tr("NTF") || filepath.m_ext == tr("GML") || filepath.m_ext == tr(""))) {
			 QMessageBox::warning(this, tr("Warning"), tr("You have selected more than one file.  Unfortunately, this feature is only currently available with NTF, GML and Tiger line files.\nPlease select a single file to import only."),
                 QMessageBox::Ok, QMessageBox::Ok);
            return;
         }
      }
      // OS files don't have GML suffix for some bizarre reason
      if (ext == tr("")) {
         ext = tr("GML");
      }
      int graph_option = MetaGraph::ADD;
      bool ok = true;

      if (ok) {
         m_communicator = new CMSCommunicator;
         if (ext != tr("RT1") && ext != tr("NTF") && ext != tr("GML")) {  // ntf, gml & rt1 use filesets (all others use standard file at a time)
            m_communicator->SetInfile( qPrintable(infiles[0]) );
         }
         if (ext != tr("MIF")) {
            CreateWaitDialog(tr("Importing file..."));
            m_communicator->SetFunction( CMSCommunicator::IMPORT );
            if (ext == tr("CAT")) {
               m_communicator->SetOption( MetaGraph::CAT | graph_option );
            }
            else if (ext == tr("DXF")) {
               m_communicator->SetOption( MetaGraph::DXF | graph_option );
            }
            else if (ext == tr("NTF")) {
               m_communicator->SetOption( MetaGraph::NTF | graph_option );
               m_communicator->SetFileSet( infiles );
            }
            else if (ext == tr("GML")) {
               m_communicator->SetOption( MetaGraph::GML | graph_option );
               m_communicator->SetFileSet( infiles );
            }
            else if (ext == tr("RT1")) {
               m_communicator->SetOption( MetaGraph::RT1 | graph_option );
               m_communicator->SetFileSet( infiles );
            }
         }
         else {
            int thedot = infiles[0].lastIndexOf('.');
            QString infile2 = infiles[0].left(thedot+1) + tr("mid");
            m_communicator->SetInfile2( qPrintable(infile2));
            CreateWaitDialog(tr("Importing file..."));
            m_communicator->SetFunction( CMSCommunicator::IMPORTMIF );
         }
         m_thread.render(this);
      }
   }
   else if (ext == tr("TXT") || ext == tr("CSV")) {
      ifstream file( infiles[0].toLatin1() );
      if (file.fail()) {
		 QMessageBox::warning(this, tr("Warning"), tr("Unable to read text file.\nPlease check that another program is not using it."),
             QMessageBox::Ok, QMessageBox::Ok);
      }
      else {
         std::unique_ptr<Communicator> comm(new ICommunicator());
         bool mapParsed = depthmapX::importFile(*m_meta_graph,
                                                file,
                                                comm.get(),
                                                filepath.m_name.toStdString(),
                                                depthmapX::ImportType::DATAMAP,
                                                (ext == tr("CSV")) ? depthmapX::ImportFileType::CSV : depthmapX::ImportFileType::TSV);
         if(mapParsed) {
            // This should have added a new data map:
            SetUpdateFlag(NEW_TABLE);

            if(graphHadNullBoundsBeforeImport) {
                SetRedrawFlag(VIEW_ALL, REDRAW_TOTAL, NEW_TABLE);
            } else {
                SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_TABLE);
            }
         }
         else {
			QMessageBox::warning(this, tr("Warning"), tr("Unable to import text file.\n \
													Unable to import text file.\n\
													Depthmap can import tab-delimited or comma separated files.\n\
													There must be some spatial data.\n\
													The spatial data can either be:\n\
													Points with X and Y values, or\n\
													points with Easting and Northing values, or\n\
													lines with X1,Y1 and X2,Y2 values"),
                                                    QMessageBox::Ok, QMessageBox::Ok);
         }
      }
   }
   else {
	  QMessageBox::warning(this, tr("Warning"), tr("Unrecognised file format.  Sorry, unable to import this file."),
         QMessageBox::Ok, QMessageBox::Ok);
   }
}

// Export file types: .txt (point files)

void QGraphDoc::OnFileExport() 
{
   if (m_communicator) {
	 QMessageBox::warning(this, tr("Notice"), tr("Sorry, cannot export as another process is running"), QMessageBox::Ok, QMessageBox::Ok);
     return;  // Locked
   }
   if (m_meta_graph->viewingNone()) {
	  QMessageBox::warning(this, tr("Notice"), tr("Sorry, cannot export as there is no data to export"), QMessageBox::Ok, QMessageBox::Ok);
      return;  // No graph to export
   }

   QString suffix;
   int mode = -1;

   int view_class = m_meta_graph->getViewClass();
   if (view_class & MetaGraph::VIEWAXIAL) {
       mode = 0;
       suffix = m_meta_graph->getDisplayedShapeGraph().getName().c_str();
   }
   else if (view_class & MetaGraph::VIEWDATA) {
       mode = 1;
       suffix = m_meta_graph->getDisplayedDataMap().getName().c_str();
   }
   else if (view_class & MetaGraph::VIEWVGA) {
      if (m_meta_graph->getDisplayedPointMap().isProcessed()) {
          mode = 2;
          suffix = tr("vga");
      }
      else {
         mode = 3;
         suffix = tr("points");
      }
   }

   if (mode == -1) {
      QMessageBox::warning(this, tr("Notice"), tr("Sorry, depthmapX does not support saving the currently displayed layer"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   suffix.replace(' ','_');

   QFilePath path(m_opened_name);
   QString defaultname = path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

   QString template_string = tr("Tab-delimited text file (*.txt)\n");
   if (mode < 3) {
	   template_string += tr("Comma separated values file (*.csv)\n");
	   template_string += tr("Graph file (*.graph)\n");
	   template_string += tr("MapInfo file (*.mif)\n");
	   template_string += tr("Pajek (*.net)\n");
   }
   template_string += tr("All files (*.*)");

   QFileDialog::Options options = 0;
   QString selectedFilter;
   QString outfile = QFileDialog::getSaveFileName(
                               0, tr("Save Output As"),
                               defaultname,
                               template_string,
                               &selectedFilter,
                               options);
   if(outfile.isEmpty())
   {
		return;
   }

   FILE* fp = fopen(outfile.toLatin1(), "wb");
   fclose(fp);

   QFilePath filepath(outfile);
   QString ext = filepath.m_ext;

   if (ext != tr("MIF") && ext != tr("GRAPH") && ext != tr("NET")) 
   {
       ofstream stream(outfile.toLatin1());
       char delimiter = '\t';
       if (ext == tr("CSV")) {
          delimiter = ',';
       }
       if (stream.fail() || stream.bad()) {
	      QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"), QMessageBox::Ok, QMessageBox::Ok);
          mode = -1;
       }

       switch (mode) {
       case 0:
          m_meta_graph->getDisplayedShapeGraph().output(stream, delimiter);
          break;
       case 1:
          m_meta_graph->getDisplayedDataMap().output(stream, delimiter);
          break;
       case 2:
          m_meta_graph->getDisplayedPointMap().outputSummary( stream, delimiter );
          break;
       case 3:
          m_meta_graph->getDisplayedPointMap().outputPoints( stream, delimiter );
          break;
       default:
          break;
       }
       stream.close();
    }
    else if (ext == tr("GRAPH")) {
        if (mode >= 3) {
           QMessageBox::warning(this, tr("Notice"), tr("Sorry, depthmapX only supports exporting VGA, axial and shape data to graph files"), QMessageBox::Ok, QMessageBox::Ok);
            return;
        }

        if (m_meta_graph->write(outfile.toStdString(), METAGRAPH_VERSION, true) != MetaGraph::OK) { // <- true writes current layer only
	        QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"), QMessageBox::Ok, QMessageBox::Ok);
        }
    }
    else if (ext == tr("NET")) {
         if (mode != 0 && mode != 2) {
            QMessageBox::warning(this, tr("Notice"), tr("Sorry, depthmapX can only export VGA graphs or shape graphs to Pajek .net files"), QMessageBox::Ok, QMessageBox::Ok);
            return;
         }
         ofstream stream(outfile.toLatin1());
         if (!stream) {
	        QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"), QMessageBox::Ok, QMessageBox::Ok);
         }
         else {
            if (mode == 0) {
               m_meta_graph->getDisplayedShapeGraph().outputNet(stream);
            }
            else if (mode == 2) {
               m_meta_graph->getDisplayedPointMap().outputNet(stream);
           }
       }
    }
    else {
         if (mode >= 3) {
            QMessageBox::warning(this, tr("Notice"), tr("Sorry, depthmapX currently only supports exporting VGA, axial and shape data to MapInfo tables"), QMessageBox::Ok, QMessageBox::Ok);
            return;
         }

         int thedot = outfile.indexOf('.');
         QString outfile2 = outfile.left(thedot+1) + tr("mid");

         ofstream miffile(outfile.toLatin1());
         if (miffile.fail() || miffile.bad()) {
	        QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"), QMessageBox::Ok, QMessageBox::Ok);
            mode = -1;
         }

         ofstream midfile(outfile2.toLatin1());
         if (midfile.fail() || midfile.bad()) {
	        QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open associated .mid file for export"), QMessageBox::Ok, QMessageBox::Ok);
            mode = -1;
         }

         if (mode == 0) {
            m_meta_graph->getDisplayedShapeGraph().outputMifMap(miffile,midfile);
         }
         else if (mode == 1) {
            m_meta_graph->getDisplayedDataMap().outputMifMap(miffile,midfile);
         }
         else if (mode == 2) {
            m_meta_graph->getDisplayedPointMap().outputMif(miffile,midfile);
        }
    }
}

void QGraphDoc::OnFileExportLinks()
{
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"), tr("Sorry, cannot export as another process is running"), QMessageBox::Ok, QMessageBox::Ok);
        return;  // Locked
    }
    if (m_meta_graph->viewingNone()) {
        QMessageBox::warning(this, tr("Notice"), tr("Sorry, cannot export as there is no data to export"), QMessageBox::Ok, QMessageBox::Ok);
        return;  // No graph to export
    }

    QString suffix;
    int mode = -1;

    int view_class = m_meta_graph->getViewClass();
    if (view_class & MetaGraph::VIEWAXIAL) {
        mode = 5;
        suffix = tr("unlinks");
    }
    else if (view_class & MetaGraph::VIEWDATA) {
        mode = 6;
        suffix = tr("links");
    }
    else if (view_class & MetaGraph::VIEWVGA) {
        if (m_meta_graph->getDisplayedPointMap().isProcessed()) {
            mode = 4;
            suffix = tr("merge_lines");
        }
    }

    if (mode == -1) {
        QMessageBox::warning(this, tr("Notice"), tr("Sorry, depthmapX does not support saving the currently displayed layer"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    suffix.replace(' ','_');

    QFilePath path(m_opened_name);
    QString defaultname = path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

    QString template_string = tr("Tab-delimited text file (*.txt)\n");
    template_string += tr("Comma separated values file (*.csv)\n");
    template_string += tr("All files (*.*)");

    QFileDialog::Options options = 0;
    QString selectedFilter;
    QString outfile = QFileDialog::getSaveFileName(
                0, tr("Save Output As"),
                defaultname,
                template_string,
                &selectedFilter,
                options);
    if(outfile.isEmpty())
    {
        return;
    }

    FILE* fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    QFilePath filepath(outfile);
    QString ext = filepath.m_ext;

    ofstream stream(outfile.toLatin1());
    char delimiter = '\t';
    if (ext == "CSV") {
      delimiter = ',';
    }
    if (stream.fail() || stream.bad()) {
      QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"), QMessageBox::Ok, QMessageBox::Ok);
      mode = -1;
    }

    switch (mode) {
    case 0:
      m_meta_graph->getDisplayedShapeGraph().output(stream, delimiter);
      break;
    case 1:
      m_meta_graph->getDisplayedDataMap().output(stream, delimiter);
      break;
    case 2:
      m_meta_graph->getDisplayedPointMap().outputSummary( stream, delimiter );
      break;
    case 3:
      m_meta_graph->getDisplayedPointMap().outputPoints( stream, delimiter );
      break;
    case 4:
      m_meta_graph->getDisplayedPointMap().outputMergeLines( stream, delimiter );
      break;
    case 5:
      // note: specific to line graphs
      m_meta_graph->getDisplayedShapeGraph().outputUnlinkPoints( stream, delimiter );
      break;
    default:
      break;
    }
    stream.close();
}

void QGraphDoc::OnAxialConnectionsExportAsDot()
{
    if (m_communicator) {
      QMessageBox::warning(this, tr("Notice"), tr("Sorry, cannot export as another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;  // Locked
    }
    if (m_meta_graph->viewingNone()) {
       QMessageBox::warning(this, tr("Notice"), tr("Sorry, cannot export as there is no data to export"), QMessageBox::Ok, QMessageBox::Ok);
       return;  // No graph to export
    }

    ShapeGraph& shapeGraph = m_meta_graph->getDisplayedShapeGraph();

    QString suffix = tr("axial_connections");

    QFilePath path(m_opened_name);
    QString defaultname = path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

    QString template_string = tr("Dot graph file (*.dot)");

    QFileDialog::Options options = 0;
    QString selectedFilter;
    QString outfile = QFileDialog::getSaveFileName(
                                0, tr("Save Output As"),
                                defaultname,
                                template_string,
                                &selectedFilter,
                                options);
    if(outfile.isEmpty())
    {
        return;
    }

    FILE* fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    ofstream stream(outfile.toLatin1());

    if (stream.fail() || stream.bad()) {
       QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"), QMessageBox::Ok, QMessageBox::Ok);
       return;
    }
    shapeGraph.writeAxialConnectionsAsDotGraph(stream);

    stream.close();
}

void QGraphDoc::OnAxialConnectionsExportAsPairCSV()
{
    if (m_communicator) {
      QMessageBox::warning(this, tr("Notice"), tr("Sorry, cannot export as another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;  // Locked
    }
    if (m_meta_graph->viewingNone()) {
       QMessageBox::warning(this, tr("Notice"), tr("Sorry, cannot export as there is no data to export"), QMessageBox::Ok, QMessageBox::Ok);
       return;  // No graph to export
    }

    ShapeGraph& shapeGraph = m_meta_graph->getDisplayedShapeGraph();

    QString suffix = tr("axial_connections");

    QFilePath path(m_opened_name);
    QString defaultname = path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

    QString template_string = tr("CSV graph file (*.csv)");

    QFileDialog::Options options = 0;
    QString selectedFilter;
    QString outfile = QFileDialog::getSaveFileName(
                                0, tr("Save Output As"),
                                defaultname,
                                template_string,
                                &selectedFilter,
                                options);
    if(outfile.isEmpty())
    {
        return;
    }

    FILE* fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    ofstream stream(outfile.toLatin1());

    if (stream.fail() || stream.bad()) {
       QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"), QMessageBox::Ok, QMessageBox::Ok);
       return;
    }
    shapeGraph.writeAxialConnectionsAsPairsCSV(stream);

    stream.close();
}

void QGraphDoc::OnSegmentConnectionsExportAsPairCSV()
{
    if (m_communicator) {
      QMessageBox::warning(this, tr("Notice"), tr("Sorry, cannot export as another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;  // Locked
    }
    if (m_meta_graph->viewingNone()) {
       QMessageBox::warning(this, tr("Notice"), tr("Sorry, cannot export as there is no data to export"), QMessageBox::Ok, QMessageBox::Ok);
       return;  // No graph to export
    }

    ShapeGraph& shapeGraph = m_meta_graph->getDisplayedShapeGraph();

    QString suffix = tr("segment_connections");

    QFilePath path(m_opened_name);
    QString defaultname = path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

    QString template_string = tr("CSV graph file (*.csv)");

    QFileDialog::Options options = 0;
    QString selectedFilter;
    QString outfile = QFileDialog::getSaveFileName(
                                0, tr("Save Output As"),
                                defaultname,
                                template_string,
                                &selectedFilter,
                                options);
    if(outfile.isEmpty())
    {
        return;
    }

    FILE* fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    ofstream stream(outfile.toLatin1());

    if (stream.fail() || stream.bad()) {
       QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"), QMessageBox::Ok, QMessageBox::Ok);
       return;
    }
    shapeGraph.writeSegmentConnectionsAsPairsCSV(stream);

    stream.close();
}

void QGraphDoc::OnPointmapExportConnectionsAsCSV()
{
    if (m_communicator) {
      QMessageBox::warning(this, tr("Notice"), tr("Sorry, cannot export as another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;  // Locked
    }
    if (m_meta_graph->viewingNone()) {
       QMessageBox::warning(this, tr("Notice"), tr("Sorry, cannot export as there is no data to export"), QMessageBox::Ok, QMessageBox::Ok);
       return;  // No graph to export
    }
    if (!(m_meta_graph->getViewClass() & MetaGraph::VIEWVGA)) {
       QMessageBox::warning(this, tr("Error"), tr("Make sure a Visibility Graph is visible"), QMessageBox::Ok, QMessageBox::Ok);
       return;  // No graph to export
    }
    if (!m_meta_graph->viewingProcessedPoints()) {
       QMessageBox::warning(this, tr("Error"),
                            tr("Make sure the visibility graph was created (Tools -> Visibility -> Make Visibility Graph)"),
                            QMessageBox::Ok, QMessageBox::Ok);
       return;  // No graph to export
    }

    PointMap& pointMap = m_meta_graph->getDisplayedPointMap();

    QString suffix = tr("connectivity");

    QFilePath path(m_opened_name);
    QString defaultname = path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

    QString template_string = tr("CSV graph file (*.csv)");

    QFileDialog::Options options = 0;
    QString selectedFilter;
    QString outfile = QFileDialog::getSaveFileName(
                                0, tr("Save Output As"),
                                defaultname,
                                template_string,
                                &selectedFilter,
                                options);
    if(outfile.isEmpty())
    {
        return;
    }

    FILE* fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    ofstream stream(outfile.toLatin1());

    if (stream.fail() || stream.bad()) {
       QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"), QMessageBox::Ok, QMessageBox::Ok);
       return;
    }
    pointMap.outputConnectionsAsCSV(stream, ",");

    stream.close();
}

void QGraphDoc::OnSwapColours() 
{
   DisplayParams displayparams;
   if (m_meta_graph->getViewClass() & MetaGraph::VIEWVGA ) {
      displayparams = m_meta_graph->getDisplayedPointMap().getDisplayParams();
      float blue = displayparams.blue;
      displayparams.blue = displayparams.red;
      displayparams.red = blue;
      m_meta_graph->getDisplayedPointMap().setDisplayParams( displayparams );
   }
   else if (m_meta_graph->getViewClass() & MetaGraph::VIEWAXIAL) {
      displayparams = m_meta_graph->getDisplayedShapeGraph().getDisplayParams();
      float blue = displayparams.blue;
      displayparams.blue = displayparams.red;
      displayparams.red = blue;
      m_meta_graph->getDisplayedShapeGraph().setDisplayParams( displayparams );
   }
   else if (m_meta_graph->getViewClass() & MetaGraph::VIEWDATA) {
      displayparams = m_meta_graph->getDisplayedDataMap().getDisplayParams();
      float blue = displayparams.blue;
      displayparams.blue = displayparams.red;
      displayparams.red = blue;
      m_meta_graph->getDisplayedDataMap().setDisplayParams( displayparams );
   }

   SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, NEW_DEPTHMAPVIEW_SETUP);
}

/////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnEditGrid() 
{
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   bool newmap = false;

   if (!m_meta_graph->PointMaps::size() || m_meta_graph->getDisplayedPointMap().isProcessed()) {
      // this can happen if there are no displayed maps -- so flag new map required:
      newmap = true;
   }
   else if (m_meta_graph->getDisplayedPointMap().getPointCount() != 0) {
      if ( QMessageBox::Yes != QMessageBox::question(this, tr("depthmapX"), tr("This will clear existing points.  Do you want to continue?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No) )
         return;
   }
   QtRegion r = m_meta_graph->SuperSpacePixel::getRegion();
   CGridDialog dlg(__max(r.width(), r.height()));
   if (QDialog::Accepted == dlg.exec())
   {
      if (newmap) {
         m_meta_graph->PointMaps::addNewMap();
      }
      m_meta_graph->setGrid( dlg.getSpacing(), Point2f(0.0f, 0.0f) );
      m_meta_graph->m_showgrid = true;
      SetUpdateFlag(NEW_TABLE);
      SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
   }
}

void QGraphDoc::OnEditFixgrid() 
{
}

void QGraphDoc::OnEditFixFill() 
{
   // only used in Developer mode
   if (m_communicator) {
	  QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   m_meta_graph->getDisplayedPointMap().fillLines();

   SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
}

// AV TV // semifilled
void QGraphDoc::OnFillPoints( const Point2f& p, int fill_type ) // semifilled = 0 (intention to use semifilled steps for part filled)
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
	  QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   if (~state & MetaGraph::LINEDATA) {
	  QMessageBox::warning(this, tr("Notice"), tr("Sorry, line drawing data must be loaded before points may be filled"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   if (~state & MetaGraph::POINTMAPS) {
	  QMessageBox::warning(this, tr("Notice"), tr("Please make grid before filling"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   if (m_meta_graph->viewingProcessed()) {
	  QMessageBox::warning(this, tr("Notice"), tr("Sorry, once the graph has been constructed, more points may not be added"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   m_communicator = new CMSCommunicator();
   m_communicator->SetSeedPoint( p );
   m_communicator->SetOption( fill_type ); // AV TV
   CreateWaitDialog(tr("Generating grid points..."));
   m_communicator->SetFunction( CMSCommunicator::MAKEPOINTS );

   m_thread.render(this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnToolsBoundaryToAxial() 
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
	  QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   if (!m_meta_graph->getDisplayedPointMap().isProcessed()) {
	  QMessageBox::warning(this, tr("Notice"), tr("Sorry, a graph must exist to construct map"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   // This is easy too... too easy... hmm... crossed-fingers, here goes:
   m_communicator = new CMSCommunicator();
   CreateWaitDialog(tr("Constructing boundary axial map..."));
   m_communicator->SetFunction( CMSCommunicator::MAKEBOUNDARYMAP );

   m_thread.render(this);
}


// convert any shape layer to any other (certain rules apply)

void QGraphDoc::OnLayerConvert() 
{
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   CMakeLayerDlg dlg;

   dlg.m_mapin = MAKELAYER_DATA;    // mapin is a data map (assume)
   dlg.m_mapout = 0;   // no possible map out

   if (m_meta_graph->viewingProcessedLines()) {
      int type = m_meta_graph->getDisplayedShapeGraph().getMapType();
      // map in is an axial map (of sorts, something with a graph...)
      dlg.m_mapin = MAKELAYER_AXIAL;
      dlg.m_origin = QString("Shape Graphs: ") + QString(m_meta_graph->getDisplayedShapeGraph().getName().c_str());
      if (type == ShapeMap::AXIALMAP) {
         // note, ShapeMap::ALLLINEMAP is deliberately excluded, as making a segment map from it would overproduce segments:
         dlg.m_mapout = MAKELAYER_DRAWING | MAKELAYER_DATA | MAKELAYER_SEGMENT;
      }
      else {
         // convex, segment, pesh and all line maps cannot be converted to segment maps:
         dlg.m_mapout = MAKELAYER_DRAWING | MAKELAYER_DATA;
      }
   }
   else if (m_meta_graph->viewingProcessedShapes()) {
      //
      dlg.m_origin = QString("Data Maps: ") + QString(m_meta_graph->getDisplayedDataMap().getName().c_str());
      // possible to go to anything apart from a data map
      dlg.m_mapout = MAKELAYER_DRAWING | MAKELAYER_AXIAL | MAKELAYER_CONVEX | MAKELAYER_SEGMENT;
   }

   if (dlg.m_mapout != 0 && QDialog::Accepted == dlg.exec()) {
      m_communicator = new CMSCommunicator();
      m_communicator->SetString( dlg.m_layer_name );
      m_communicator->SetOption( dlg.m_keeporiginal ? 1 : 0, 0);  // <- option 0 used for retain original flag
      m_communicator->SetOption( dlg.m_push_values ? 1 : 0, 1);   // <- option 1 used for push values flag
      //
      if (dlg.m_mapout == MAKELAYER_DRAWING) {
         // this one new: data or graph (any sort) -> drawing
         m_communicator->SetOption( dlg.m_mapin == MAKELAYER_DATA ? 0 : 1, 1); // <- option 1 in this case signifies base as graph or data
         // (option 1 overidden, as data cannot be pushed to a drawing layer)
         CreateWaitDialog(tr("Constructing segment map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKEDRAWING );
      }
      if (dlg.m_mapout == MAKELAYER_DATA) {
         // this one new: graph (any sort) -> data
         CreateWaitDialog(tr("Constructing segment map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKEGATESMAP );
      }
      else if (dlg.m_mapout == MAKELAYER_CONVEX) {
         // this one new: data -> convex map
         CreateWaitDialog(tr("Constructing convex map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKECONVEXMAP );
      }
      else if (dlg.m_mapout == MAKELAYER_AXIAL) {
         // this one originally data -> axial map
         CreateWaitDialog(tr("Constructing axial map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKEUSERMAPSHAPE );
      }
      else if (dlg.m_mapout == MAKELAYER_SEGMENT) { 
         if (dlg.m_mapin == MAKELAYER_AXIAL) {
            // this one originally axial -> segment map
            // use option 2 to specify percentage removal
            m_communicator->SetOption( dlg.m_remove_stubs ? dlg.m_percentage : 0, 2);
            CreateWaitDialog(tr("Constructing segment map..."));
            m_communicator->SetFunction( CMSCommunicator::MAKESEGMENTMAP );
         }
         else {
            // this one originally data -> segment map
            CreateWaitDialog(tr("Constructing segment map..."));
            m_communicator->SetFunction( CMSCommunicator::MAKEUSERSEGMAPSHAPE );
         }
      }
	  m_thread.render(this);      
   }
}

void QGraphDoc::OnLayerConvertDrawing() 
{
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   CMakeLayerDlg dlg;

   dlg.m_mapin = MAKELAYER_DRAWING;    // mapin is a drawing map
   dlg.m_mapout = MAKELAYER_DATA | MAKELAYER_AXIAL | MAKELAYER_CONVEX | MAKELAYER_SEGMENT;
   dlg.m_origin = QString(tr("Drawing Layers: All Displayed"));

   if (QDialog::Accepted == dlg.exec()) {
      m_communicator = new CMSCommunicator();
      m_communicator->SetString( dlg.m_layer_name );
      m_communicator->SetOption( dlg.m_keeporiginal ? 1 : 0, 0);  // <- option 0 used for retain original flag
      m_communicator->SetOption( -1, 1);  // this is used to distinguish between 0 or 1 used when converting data layer
      //
      if (dlg.m_mapout == MAKELAYER_DATA) {
         // this one originally drawing -> data map
         CreateWaitDialog(tr("Constructing data map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKEGATESMAP );
      }
      else if (dlg.m_mapout == MAKELAYER_AXIAL) {
         // this one originally drawing -> axial map
         CreateWaitDialog(tr("Constructing axial map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKEUSERMAP );
      }
      else if (dlg.m_mapout == MAKELAYER_CONVEX) {
         // this one new: drawing -> convex map
         CreateWaitDialog(tr("Constructing convex map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKECONVEXMAP );
      }
      else if (dlg.m_mapout == MAKELAYER_SEGMENT) {
         // this one originally drawing -> seg map
         CreateWaitDialog(tr("Constructing segment map..."));
         m_communicator->SetFunction( CMSCommunicator::MAKEUSERSEGMAP );
      }
      m_thread.render(this);     
   }
}


// arbitrary isovist
void QGraphDoc::OnMakeIsovist(const Point2f& seed, double angle)
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
	  QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   if (~state & MetaGraph::LINEDATA) {
	  QMessageBox::warning(this, tr("Notice"), tr("Sorry, line drawing data must be loaded before an isovist can be constructed"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   CIsovistPathDlg dlg;
   if (dlg.exec() == QDialog::Accepted)
   {
      double fov = dlg.fov_angle;
      // This is easy too... too easy... hmm... crossed-fingers, here goes:
      m_communicator = new CMSCommunicator();
      m_communicator->SetSeedPoint( seed );
      m_communicator->SetSeedAngle( angle );
      m_communicator->SetSeedFoV( fov );
      CreateWaitDialog(tr("Constructing BSP tree to calculate isovist..."));
      m_communicator->SetFunction( CMSCommunicator::MAKEISOVIST );

	  m_thread.render(this);
   }
}

void QGraphDoc::OnToolsIsovistpath()
{
   int state = m_meta_graph->getState();
   if (state & MetaGraph::LINEDATA) {
      int view = m_meta_graph->getViewClass();
      if ((view & (MetaGraph::VIEWDATA | MetaGraph::VIEWAXIAL)) != 0 && m_meta_graph->isSelected()) {
         CIsovistPathDlg dlg;
		 if (dlg.exec() == QDialog::Accepted) {
            double angle = dlg.fov_angle;
            m_communicator = new CMSCommunicator();
            m_communicator->SetSeedAngle( angle );
            CreateWaitDialog(tr("Constructing BSP tree to calculate isovists..."));
            m_communicator->SetFunction( CMSCommunicator::MAKEISOVISTPATH );
            m_thread.render(this);
         }
      }
      else {
         // an explanation of what you need to do as it isn't obvious!
		  QMessageBox::warning(this, tr("Notice"), tr("To get a path you will need some lines for a path.\nTo do so, have an active map which is either a data map or line map of some sort.\nNext, select lines or polylines that you wish to turn into isovist paths.\nThere is no need for you to have a grid set.\n"), QMessageBox::Ok, QMessageBox::Ok);
      }
   }
}



// all line map
void QGraphDoc::OnToolsAxialMap(const Point2f& seed) 
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
	  QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   if (~state & MetaGraph::LINEDATA) {
	  QMessageBox::warning(this, tr("Notice"), tr("Sorry, line drawing data must be loaded before an axial map can be constructed"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   if (m_meta_graph->hasAllLineMap()) {
	  int ret = QMessageBox::warning(this, tr("My Application"),
										tr("This will overwrite your existing all-line map, do you want to continue?"),
										QMessageBox::Yes | QMessageBox::No
										| QMessageBox::Yes,
										QMessageBox::No);
	  if(ret != QMessageBox::Yes) return;
   }

   // This is easy too... too easy... hmm... crossed-fingers, here goes:
   m_communicator = new CMSCommunicator();
   m_communicator->SetSeedPoint( seed );
   CreateWaitDialog(tr("Constructing all line axial map..."));
   m_communicator->SetFunction( CMSCommunicator::MAKEALLLINEMAP );
   m_thread.render(this);
}

// fewest line map
void QGraphDoc::OnToolsMakeFewestLineMap() 
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   if (~state & MetaGraph::SHAPEGRAPHS) {
      QMessageBox::warning(this, tr("Warning"), tr("Sorry, all line map must exist in order to construct fewest line map"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   else if (!m_meta_graph->hasAllLineMap()) {
      QMessageBox::warning(this, tr("Warning"), tr("Sorry, all line map must exist in order to construct fewest line map"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   int replace = 0;

   // check for existing axial maps and warn user if necessary:
   ShapeGraphs& axialmaps = m_meta_graph->getShapeGraphs();
   if (axialmaps.getMapRef("Fewest-Line Map (Subsets)") != paftl::npos ||
       axialmaps.getMapRef("Fewest Line Map (Subsets)") != paftl::npos ||
       axialmaps.getMapRef("Fewest-Line Map (Minimal)") != paftl::npos ||
       axialmaps.getMapRef("Fewest Line Map (Minimal)") != paftl::npos) {
      CPromptReplace dlg;
      dlg.m_message = tr("There is already a fewest line axial map, would you like to add to it or replace it?");
      int result = dlg.exec();
	  if (result == QDialog::Rejected) {
         return;
      }
      else if (result == 2) {
         replace = 1;
      }
   }

   // This is easy too... too easy... hmm... crossed-fingers, here goes:
   m_communicator = new CMSCommunicator();
   CreateWaitDialog(tr("Constructing fewest line axial map..."));
   m_communicator->SetFunction( CMSCommunicator::MAKEFEWESTLINEMAP );
   m_communicator->SetOption( replace );
   m_thread.render(this);
}

void QGraphDoc::OnToolsRunAxa() 
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   CAxialAnalysisOptionsDlg dlg(m_meta_graph);

   if (QDialog::Accepted == dlg.exec()) {
      m_communicator = new CMSCommunicator();
      CreateWaitDialog(tr("Performing axial line analysis..."));
      m_communicator->SetFunction( CMSCommunicator::AXIALANALYSIS );
      m_thread.render(this);
   }
}

void QGraphDoc::OnToolsRunSeg() 
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   CSegmentAnalysisDlg dlg(m_meta_graph);

   if (QDialog::Accepted == dlg.exec()) {
      m_communicator = new CMSCommunicator();
      CreateWaitDialog(tr("Performing segment line analysis..."));
      m_communicator->SetFunction( CMSCommunicator::SEGMENTANALYSIS );
      m_thread.render(this);
   }
}

void QGraphDoc::OnToolsTopomet()
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
      QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   CTopoMetDlg dlg;

   if (QDialog::Accepted == dlg.exec()) {
      m_communicator = new CMSCommunicator();
      ((MainWindow*)m_mainFrame)->m_options.output_type = dlg.m_topological;
      ((MainWindow*)m_mainFrame)->m_options.radius = dlg.m_dradius;
      ((MainWindow*)m_mainFrame)->m_options.sel_only = dlg.m_selected_only;
      if (dlg.m_topological == 0) {
         CreateWaitDialog(tr("Performing topological analysis..."));
      }
      else {
         CreateWaitDialog(tr("Performing metric analysis..."));
      }
      m_communicator->SetFunction( CMSCommunicator::TOPOMETANALYSIS );
      m_thread.render(this);
   }
}

////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnToolsAxialClearLinks() 
{
   if(QMessageBox::Yes == QMessageBox::warning(this, tr("depthmapX"),
										tr("Are you sure you want to clear all links and unlinks?"),
										QMessageBox::Yes | QMessageBox::No
										| QMessageBox::Yes,
										QMessageBox::No))
   {
      m_meta_graph->getDisplayedShapeGraph().clearLinks();
      // if currently in join mode, then redraw:
      SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA );
   }
}

///////////////////////////////////////////////////////////////////////////////////////////

// New agent functionality:

void QGraphDoc::OnToolsAgentRun() 
{
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   AgentEngine& eng = m_meta_graph->getAgentEngine();

   // set up eng here...
   if (!eng.size()) {
      eng.push_back(AgentSet());
   }

   CAgentAnalysisDlg dlg;
   dlg.m_timesteps = eng.m_timesteps;
   dlg.m_release_rate = eng.tail().m_release_rate;
   dlg.m_release_location = eng.tail().m_release_locations.size() ? 1 : 0;
   dlg.m_frames = eng.tail().m_lifetime;
   if (eng.tail().m_vbin == -1) {
      dlg.m_fov = 32;
   }
   else {
      dlg.m_fov = eng.tail().m_vbin * 2 + 1;
   }
   dlg.m_steps = eng.tail().m_steps;
   dlg.m_record_trails = eng.m_record_trails;
   dlg.m_trail_count = eng.m_trail_count;
   dlg.m_names.push_back("<None>");
   for (size_t i = 0; i < m_meta_graph->getDataMaps().getMapCount(); i++) {
       dlg.m_names.push_back(m_meta_graph->getDataMaps().getMap(i).getName());
   }
   dlg.m_gatelayer = eng.m_gatelayer;

   if (QDialog::Accepted != dlg.exec()) {
      return;
   }

   eng.m_timesteps = dlg.m_timesteps;
   eng.tail().m_release_rate = dlg.m_release_rate;
   eng.tail().m_lifetime = dlg.m_frames;
   if (dlg.m_fov == 32) {
      eng.tail().m_vbin = -1;
   }
   else {
      eng.tail().m_vbin = (dlg.m_fov - 1) / 2;
   }
   eng.tail().m_steps = dlg.m_steps;
   if (dlg.m_occlusion == 0) {
      eng.tail().m_sel_type = AgentProgram::SEL_STANDARD;
   }
   else if (dlg.m_occlusion == 1) {
      eng.tail().m_sel_type = AgentProgram::SEL_LOS;
   }
   else if (dlg.m_occlusion == 2) {
      eng.tail().m_sel_type = AgentProgram::SEL_LOS_OCC;
   }
   else {
      // (dlg.m_occlusion - 2) should be from 1...8
      eng.tail().m_sel_type = AgentProgram::SEL_OCCLUSION + (dlg.m_occlusion - 2);
   }
   if (dlg.m_release_location == 1) {
      std::set<int> selected = m_meta_graph->getSelSet();
      std::copy(selected.begin(), selected.end(), std::back_inserter(eng.tail().m_release_locations));;
   }
   else {
      eng.tail().m_release_locations.clear();
   }
   eng.m_gatelayer = dlg.m_gatelayer;

   // note, trails currently per run, but output per engine
   if (dlg.m_record_trails) {
      eng.m_record_trails = true;
      eng.m_trail_count = dlg.m_trail_count;
   }

   // then go:

   m_communicator = new CMSCommunicator();
   CreateWaitDialog(tr("Performing agent analysis..."));
   m_communicator->SetFunction( CMSCommunicator::AGENTANALYSIS );

   m_thread.render(this);
}

// some evo agent code... not sure if this works or not!

void QGraphDoc::OnEvoAgent() 
{
/*   if (!m_evoagent) {
      m_evoagent = (CEvoAgent *) AfxBeginThread(RUNTIME_CLASS(CEvoAgent));
      if (!m_evoagent) {
         AfxMessageBox(tr("An error occurred trying to start the agent simulation module"));
         return;
      }
      else {
         CEvoAgentSetup dlg;
         if (dlg.DoModal() == IDOK) {
            m_evoagent->Init(this,dlg.m_evolveapply,dlg.m_filename,dlg.m_seed);
            m_evoagent->PostThreadMessage(WM_DMP_RUN,0,0);
         }
         else {
            m_evoagent->PostThreadMessage(WM_DMP_FINISHED_MESSAGE,0,0);
            m_evoagent = NULL;
         }
      }
   }
   else {
      // turn off pause (typically pauses after evaluation run)
      m_evoagent->m_paused = false;
      m_evoagent->PostThreadMessage(WM_DMP_RUN,0,0);
   }
   */
}

/////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnEditUndo() 
{
	if (!m_meta_graph->canUndo()) {
		QMessageBox::warning(this, tr("Warning"), tr("Sorry, no undo available for this map"), QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	m_meta_graph->undo();
	modifiedFlag = true;
	SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA );
}


void QGraphDoc::OnEditClear() 
{
   int state = m_meta_graph->getState();

   int editable = m_meta_graph->isEditable();
   if (editable == MetaGraph::NOT_EDITABLE) {
		QMessageBox::warning(this, tr("Warning"), tr("Cannot delete: the geometry forming this graph cannot be edited."), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   else if (editable == MetaGraph::EDITABLE_OFF) {
		QMessageBox::warning(this, tr("Warning"), tr("Cannot delete: this graph is currently uneditable."), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   bool modified = false;
   if (m_meta_graph->viewingUnprocessedPoints()) {
      modified = m_meta_graph->clearPoints();
   }
   else if (m_meta_graph->viewingProcessedLines()) {
      modified = m_meta_graph->getDisplayedShapeGraph().removeSelected();
   }
   else if (m_meta_graph->viewingProcessedShapes()) {
      modified = m_meta_graph->getDisplayedDataMap().removeSelected();
   }

   if(modified) {
       modifiedFlag = true;
   }

   SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA );
}


/////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnToolsMakeGraph() 
{
   int state = m_meta_graph->getState();
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   if (!m_meta_graph->viewingUnprocessedPoints()) {
	   QMessageBox::warning(this, tr("Warning"), tr("Sorry, you need an unprocessed set of points to make the visibility graph"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   /*if (!CheckMemory()) {
      return;
   }*/

   CMakeOptionsDlg dlg;
   dlg.m_boundarygraph = false;
   if (dlg.exec() != QDialog::Accepted) {
      return;
   }
   if (dlg.m_boundarygraph) {
      m_make_algorithm = 1;
   }
   else {
      m_make_algorithm = 0;
   }
   if (dlg.m_restrict_visibility) {
      m_make_maxdist = dlg.m_maxdist;
   }
   else {
      m_make_maxdist = -1.0;
   }
 
   m_communicator = new CMSCommunicator();
   CreateWaitDialog(tr("Constructing graph..."));
   m_communicator->SetFunction( CMSCommunicator::MAKEGRAPH );

   m_thread.render(this);
}

/////////////////////////////////////////////////////////////////////////////


void QGraphDoc::OnVGAOptions() 
{
   COptionsDlg dlg;

   dlg.m_layer_names.push_back("<None>");
   for (size_t i = 0; i < m_meta_graph->getDataMaps().getMapCount(); i++) {
       dlg.m_layer_names.push_back(m_meta_graph->getDataMaps().getMap(i).getName());
   }

   dlg.exec();
}

void QGraphDoc::OnToolsRun() 
{
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   // This is easy!
   COptionsDlg dlg;

   dlg.m_layer_names.push_back("<None>");
   for (size_t i = 0; i < m_meta_graph->getDataMaps().getMapCount(); i++) {
       dlg.m_layer_names.push_back(m_meta_graph->getDataMaps().getMap(i).getName());
   }

   if (QDialog::Accepted != dlg.exec()) {
      return;
   }

   // This is easy too... too easy... hmm... crossed-fingers, here goes:
   m_communicator = new CMSCommunicator();
   CreateWaitDialog(tr("Analysing graph..."));
   m_communicator->SetFunction( CMSCommunicator::ANALYSEGRAPH );

   m_thread.render(this);
}

void QGraphDoc::OnToolsPD() 
{
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   if (m_meta_graph->viewingProcessed()) {
      if (m_meta_graph->isSelected()) {

         // This is easy too... too easy... hmm... crossed-fingers, here goes:
         m_communicator = new CMSCommunicator();
         CreateWaitDialog(tr("Calculating step depth..."));
         m_communicator->SetFunction( CMSCommunicator::POINTDEPTH );

         m_thread.render(this);
      }
   }
}


void QGraphDoc::OnToolsMPD() 
{
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   if (m_meta_graph->viewingProcessedPoints() || (m_meta_graph->viewingProcessedLines() && m_meta_graph->getDisplayedShapeGraph().isSegmentMap())) {
      if (m_meta_graph->isSelected()) {

         // This is easy too... too easy... hmm... crossed-fingers, here goes:
         m_communicator = new CMSCommunicator();
         CreateWaitDialog(tr("Calculating metric depth..."));
         m_communicator->SetFunction( CMSCommunicator::METRICPOINTDEPTH );

         m_thread.render(this);
      }
   }
}

void QGraphDoc::OnToolsAPD() 
{
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   if (m_meta_graph->viewingProcessedPoints()) {
      if (m_meta_graph->isSelected()) {

         // This is easy too... too easy... hmm... crossed-fingers, here goes:
         m_communicator = new CMSCommunicator();
         CreateWaitDialog(tr("Calculating angular depth..."));
         m_communicator->SetFunction( CMSCommunicator::ANGULARPOINTDEPTH );

         m_thread.render(this);
      }
   }
}
/*
void QGraphDoc::OnUpdateToolsTPD()//CCmdUI *pCmdUI)
{
   // segment maps only (with selection)
   if (m_meta_graph->viewingProcessedLines() && m_meta_graph->getDisplayedShapeGraph().isSegmentMap() && m_meta_graph->isSelected()) {
      pCmdUI->Enable(TRUE);
   }
   else {
      pCmdUI->Enable(FALSE);
   }	
}*/

void QGraphDoc::OnToolsTPD()
{
   if (m_communicator) {
	   QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   if (m_meta_graph->viewingProcessedLines() && m_meta_graph->getDisplayedShapeGraph().isSegmentMap()) {
      if (m_meta_graph->isSelected()) {

         // This is easy too... too easy... hmm... crossed-fingers, here goes:
         m_communicator = new CMSCommunicator();
         CreateWaitDialog(tr("Calculating topological depth..."));
         m_communicator->SetFunction( CMSCommunicator::TOPOLOGICALPOINTDEPTH );

         m_thread.render(this);
      }
   }
}

void QGraphDoc::OnBinDisplay() 
{
   if (m_communicator) {
      QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   if (m_meta_graph->viewingProcessedPoints()) {
      if (m_meta_graph->isSelected()) {

         // This is easy too... too easy... hmm... crossed-fingers, here goes:
         m_communicator = new CMSCommunicator();
         CreateWaitDialog(tr("Showing bins..."));
         m_communicator->SetFunction( CMSCommunicator::BINDISPLAY );

         m_thread.render(this);
      }
   }
}

/////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnToolsAxialLines() 
{
   if (m_communicator) {
      QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   // This is easy too... too easy... hmm... crossed-fingers, here goes:
   m_communicator = new CMSCommunicator();
   CreateWaitDialog(tr("Analysing graph..."));
   m_communicator->SetFunction( CMSCommunicator::MAKEAXIALLINES );
   m_thread.render(this);
}

/////////////////////////////////////////////////////////////////////////////
static int sequenceNumber = 1;
bool QGraphDoc::OnNewDocument()
{
	m_base_title = tr("Untitled%1").arg(sequenceNumber++);
	
	SetRemenuFlag(QGraphDoc::VIEW_ALL, true);
	return TRUE;
}

int QGraphDoc::OnOpenDocument(char* lpszPathName) 
{
   if (m_communicator) {
      QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return FALSE;
   }

   m_opened_name = QString(lpszPathName);

   int ok = m_meta_graph->read( lpszPathName );
   QFilePath path(m_opened_name);

   SetUpdateFlag(QGraphDoc::NEW_FILE,false);
   int ret = FALSE;

   switch (ok) {
   case MetaGraph::OK:
	  m_base_title = path.m_name;
      ret = TRUE;
      break;
   case MetaGraph::WARN_BUGGY_VERSION:
      QMessageBox::warning(this, tr("Warning"), tr("this graph was made with a version of depthmapX that contained slight errors"), QMessageBox::Ok, QMessageBox::Ok);
      ret = TRUE;
      break;
   case MetaGraph::WARN_CONVERTED:
      QMessageBox::warning(this, tr("Warning"), tr("Warning: this graph was made with an older version of depthmapX.\n" \
          "Some aspects of the graph may not have been translated to the new depthmapX properly."), QMessageBox::Ok, QMessageBox::Ok);
      ret = TRUE;
      break;
   case MetaGraph::NOT_A_GRAPH:
      QMessageBox::warning(this, tr("Warning"), tr("Unable to open graph: not recognised as a graph file."), QMessageBox::Ok, QMessageBox::Ok);
      break;
   case MetaGraph::DAMAGED_FILE:
      QMessageBox::warning(this, tr("Warning"), tr("Unable to open graph: the graph file is damaged."), QMessageBox::Ok, QMessageBox::Ok);
      break;
   case MetaGraph::DISK_ERROR:
      QMessageBox::warning(this, tr("Warning"), tr("Unable to open graph: an error occurred while trying to read from the disk."), QMessageBox::Ok, QMessageBox::Ok);
      break;
   case MetaGraph::NEWER_VERSION:
      QMessageBox::warning(this, tr("Warning"), tr("Unable to open graph: this graph has been written by a newer version of depthmapX."), QMessageBox::Ok, QMessageBox::Ok);
      break;
   case MetaGraph::DEPRECATED_VERSION:
      QMessageBox::warning(this, tr("Warning"), tr("Unable to open graph: this is a graph file format not supported by this version of depthmapX."), QMessageBox::Ok, QMessageBox::Ok);
      break;
   default:
      {
         std::string err = dXstring::formatString(ok);
         QMessageBox::warning(this, tr("Warning"), tr("Unable to open graph: error number "), QMessageBox::Ok, QMessageBox::Ok);
      }
      break;
   }

   return ret;
}

bool QGraphDoc::OnFileSave()
{
	QString newName = m_opened_name;
	if (newName.isEmpty()) {
        newName = m_base_title + tr(".graph");
		QFileDialog::Options options = 0;
		QString outfile = QFileDialog::getSaveFileName(
								   0, tr("Save As"),
								   newName,
								   tr("Graph file (*.graph)\nAll files (*.*)"),
								   0,
								   options);
        if (outfile.isEmpty()) return false;

		m_opened_name = outfile;

        FILE* fp = fopen(m_opened_name.toLatin1(), "wb");
		fclose(fp);

        OnSaveDocument(outfile);

        QFilePath path(m_opened_name);
        m_base_title = path.m_name;
        return true;
	}
	
    OnSaveDocument(newName);
    return true;
}

bool QGraphDoc::OnFileSaveAs()
{
   // This is based on Microsoft's "DoSave" function, but
   // it allows two options for saving: one as the current 
   // graph format, and one as the original

	QString newName = m_opened_name;
 	if (newName.isEmpty()) {
       newName = m_base_title + tr(".graph");
	}

	QFileDialog::Options options = 0;
	QString outfile = QFileDialog::getSaveFileName(
                               0, tr("Save As"),
                               newName,
                               tr("Graph file (*.graph)\ndepthmapX 8 graph (*.graph)\nAll files (*.*)"),
                               0,
                               options);

	if (outfile.isEmpty())
        return false;

    FILE* fp = fopen(outfile.toLatin1(), "wb");
	fclose(fp);

    OnSaveDocument(outfile.toLatin1());
	
	// reset the title and change the document name
    m_opened_name = outfile;
    QFilePath path(m_opened_name);
    m_base_title = path.m_name;
    return true;
}

int QGraphDoc::OnSaveDocument(QString lpszPathName) 
{
   // default: save in current version format
   int version = m_meta_graph->getVersion();
   // version == -1 is unsaved, which is fine to save in current version
   if (version != -1 && version != METAGRAPH_VERSION) {
      if(QMessageBox::Yes == QMessageBox::question(this, tr("depthmapX"), tr("This will overwrite the file with the latest graph format, which may not be readable by previous versions of depthmapX.\nDo you want to overwrite it?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No))
         return OnSaveDocument(lpszPathName, METAGRAPH_VERSION);
      else return TRUE;
   }
   return OnSaveDocument(lpszPathName, METAGRAPH_VERSION);
}

int QGraphDoc::OnSaveDocument(QString lpszPathName, int version)
{
   if (m_communicator) {
      QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return FALSE;
   }

   if (m_meta_graph->getState() & MetaGraph::BUGGY) {
      if(QMessageBox::No == QMessageBox::question(this, tr("depthmapX"), tr("This graph file was created by a version of depthmapX with slight errors.  Are you sure you want to save it?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No))
         return FALSE;
   }

   modifiedFlag = true;

   int ok = m_meta_graph->write( lpszPathName.toStdString(), version );
   if (ok == MetaGraph::OK) {
	   modifiedFlag = false;
      return TRUE;
   }
   else if (ok == MetaGraph::DISK_ERROR) {
      QMessageBox::warning(this, tr("Warning"), tr("Unable to save graph: is there enough disk space?"), QMessageBox::Ok, QMessageBox::Ok);
   }

   return FALSE;
}

bool QGraphDoc::OnCloseDocument(int index) 
{
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("depthmapX is processing, please kill the process to continue"), QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    int i;
    for (i = 1; i < VIEW_TYPES; i++) if (m_view[i]) break;


    if(modifiedFlag)
    {
        int result = QMessageBox::question(this, tr("depthmapX"), tr("Do you want to save the changes?"), QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Cancel);
        if(QMessageBox::Yes == result)
        {
            OnFileSave();
            if (i == VIEW_TYPES)
            {
                modifiedFlag = false;
                QApplication::postEvent((QObject*)m_mainFrame, new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void*)this, CONTROLS_DESTROYALL));
            }
            return true;
        }
        else if (QMessageBox::No == result)
        {
            if (i == VIEW_TYPES)
            {
                modifiedFlag = false;
                QApplication::postEvent((QObject*)m_mainFrame, new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void*)this, CONTROLS_DESTROYALL));
            }
            return true;
        }
        else
            return false;
    }
    if (i == VIEW_TYPES)
    {
        QApplication::postEvent((QObject*)m_mainFrame, new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void*)this, CONTROLS_DESTROYALL));
    }
    return true;
}

void QGraphDoc::OnAddGate() 
{
   if (m_communicator) {
      QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   if (m_meta_graph->getViewClass() & MetaGraph::VIEWVGA) {
      // try the new thingy:
      if (m_meta_graph->convertPointsToShape()) {
         // redraw all as change of view is affected here:
         // (also possibly even the sidebar menu:
//         GetApp()->GetMainWnd()->PostMessage( WM_DMP_FOCUS_GRAPH, (WPARAM) this, QGraphDoc::CONTROLS_LOADGRAPH );
         SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DEPTHMAPVIEW_SETUP );
      }
      else {
		  QMessageBox::warning(this, tr("Warning"), tr("There was an error trying to convert points to a shape.\nPlease check that you have selected a single polygon without holes.") );
      }
   }
}

void QGraphDoc::OnPushToLayer() 
{
   if (m_meta_graph->viewingProcessed()) {
      int toplayerclass = (m_meta_graph->getViewClass() & MetaGraph::VIEWFRONT);
      std::string origin_layer;
      std::string origin_attribute;
      pqmap<IntPair,std::string> names;
      // I'm just going to allow push from any layer to any other layer
      // (apart from VGA graphs, which cannot map onto themselves
      if (toplayerclass == MetaGraph::VIEWVGA) {
         // bit clunky just to get two names out...
         PointMap& map = m_meta_graph->getDisplayedPointMap();
         origin_layer = std::string("Visibility Graphs: ") + map.getName();
         origin_attribute = map.getAttributeTable().getColumnName(map.getDisplayedAttribute());
      }
      else if (toplayerclass == MetaGraph::VIEWAXIAL) {
         // bit clunky just to get two names out...
         ShapeGraph& map = m_meta_graph->getDisplayedShapeGraph();
         origin_layer = std::string("Shape Graphs: ") + map.getName();
         origin_attribute = map.getAttributeTable().getColumnName(map.getDisplayedAttribute());
      }
      else if (toplayerclass == MetaGraph::VIEWDATA) {
         // bit clunky just to get two names out...
         ShapeMap& map = m_meta_graph->getDisplayedDataMap();
         origin_layer = std::string("Data Maps: ") + map.getName();
         origin_attribute = map.getAttributeTable().getColumnName(map.getDisplayedAttribute());
      }
      else {
         // eek!
		  QMessageBox::warning(this, tr("Warning"), tr("No valid map displayed from which to push data"), QMessageBox::Ok, QMessageBox::Ok);
         return;
      }
      // layers to push to:
      size_t i;
      ShapeMaps<ShapeMap>& datamaps = m_meta_graph->getDataMaps();
      for (i = 0; i < datamaps.getMapCount(); i++) {
         if (toplayerclass != MetaGraph::VIEWDATA || i != datamaps.getDisplayedMapRef()) {
            names.add(IntPair(MetaGraph::VIEWDATA,int(i)),std::string("Data Maps: ") + datamaps.getMap(i).getName());
         }
      }
      ShapeGraphs& shapegraphs = m_meta_graph->getShapeGraphs();
      for (i = 0; i < shapegraphs.getMapCount(); i++) {
         if (toplayerclass != MetaGraph::VIEWAXIAL || i != shapegraphs.getDisplayedMapRef()) {
            names.add(IntPair(MetaGraph::VIEWAXIAL,int(i)),std::string("Shape Graphs: ") + shapegraphs.getMap(i).getName());
         }
      }
      for (i = 0; i < m_meta_graph->PointMaps::size(); i++) {
         // note 1: no VGA graph can push to another VGA graph (point onto point transforms)
         // note 2: I simply haven't written "axial" -> vga yet, not that it can't be possible (e.g., "axial" could actually be a convex map)
         if (toplayerclass != MetaGraph::VIEWVGA && toplayerclass != MetaGraph::VIEWAXIAL) {
            names.add(IntPair(MetaGraph::VIEWVGA,int(i)),std::string("Visibility Graphs: ") + m_meta_graph->PointMaps::at(i).getName());
         }
      }
      CPushDialog dlg(names);
      dlg.m_origin_layer = QString(origin_layer.c_str());
      dlg.m_origin_attribute = QString(origin_attribute.c_str());
	  if (QDialog::Accepted == dlg.exec()) {
         m_communicator = new CMSCommunicator;   // dummy value to prevent draw while this operation is in progress
         // now have to separate vga and axial layers again:
         int sel = dlg.m_layer_selection;
         IntPair dest = names.key(sel);
//         CWaitCursor c;
         m_meta_graph->pushValuesToLayer(dest.a, dest.b, dlg.m_function, dlg.m_count_intersections);
         delete m_communicator;
         m_communicator = NULL;
         SetUpdateFlag(NEW_TABLE);
      }
   }
}

///////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnRedtest() 
{
   // OzlemSpecial2
   // chop rays if they intersect other buildings,
   // exclude them altogether if there is just a short gap between buildings

   // OzlemSpecial3
   // chop rays according to encountered feature codes

   // expect this to be the rays:
   int a = (int) m_meta_graph->getDataMaps().getMapRef("Axis Rays");

   // expect this to be the buildings:
   int b = 2;  // hmm -- just guess a number! (0 for OzlemSpecial2 and 2 for OzlemSpecial3 as designed to be run)

   m_meta_graph->getDataMaps().getMap(a).ozlemSpecial3( m_meta_graph->getDataMaps().getMap(b) );

   SetUpdateFlag(QGraphDoc::NEW_DATA);
   SetRedrawFlag(QGraphDoc::VIEW_ALL, QGraphDoc::REDRAW_POINTS, QGraphDoc::NEW_DATA );
}

void QGraphDoc::OnGreentest() 
{
   // OzlemSpecial5
   // mark up buildings according to exposure types

   // expect this to be the rays:
   int a = (int) m_meta_graph->getDataMaps().getMapRef("Axis Rays");

   // expect this to be the buildings:
   int b = 0;  // hmm -- just guess a number! (0 for OzlemSpecial5)

   m_meta_graph->getDataMaps().getMap(a).ozlemSpecial5( m_meta_graph->getDataMaps().getMap(b) );
  
//   m_meta_graph->getDisplayedDataMap().ozlemSpecial6();
}

void QGraphDoc::OnPinktest() 
{
   // note: expects 0 to be points, 1 to be lines
   m_meta_graph->getDataMaps().getMap(0).ozlemSpecial7( m_meta_graph->getDataMaps().getMap(1) );
}

void QGraphDoc::OnTestButton() 
{
/*   AfxMessageBox(tr("test!"));
   AttributeTable *tester = new AttributeTable();
   for (int i = 0; i < 1000000; i++) {
      tester->insertRow(i);
   }
   delete tester;
   AfxMessageBox(tr("tested!"));
*/

   /*
   if (saveDialog.DoModal() == IDOK) {
      CWaitCursor;
      QString outfile = saveDialog.GetPathName();
      ofstream stream(outfile);
      stream << "Building\tRoad1\tRoad2\tangle" << endl;
      m_meta_graph->getDataMaps().at(1).ozlemSpecial( stream, m_meta_graph->getDataMaps().at(0));
   }
*/
   /*
   ifstream file("F:\\alasdair\\apps\\saladtester\\paths.txt");
                                                                   // <<
   m_evolved_paths.clear();
   for (int i = 0; i < 19; i++) {
      m_evolved_paths.push_back(pqvector<Point2f>());
      for (int j = 0; j < 1200; j++) {
         Point2f p;
         file >> p.x >> p.y;
         m_evolved_paths.tail().push_back(p);
      }
   }
*/

   /*
   int counter = 0;

   int seed = 1;
   pafsrand(seed);
   int nextrand;

   int buckets[100];
   for (int i = 0; i < 100; i++)
   {
      buckets[i] = 0;
   }

   do {
      nextrand = pafrand();
      buckets[nextrand % 100] += 1;
   } while (++counter < PAF_RAND_MAX * 10);

   ofstream data("data.txt");

   for (int j = 0; j < 100; j++) {
      data << buckets[j] << endl;
   }

   if (counter == PAF_RAND_MAX) {
      AfxMessageBox(tr("blah!"));
   }
   else {
      AfxMessageBox(tr("wah!"));
   }
*/
   //m_meta_graph->highlightBorder();
   //SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH);
/*   if (m_agent_manager && m_agent_manager->isPaused()) {
      Evoecomorph& evo = m_agent_manager->getEvoecomorph();
      double val = evo.getLatestFitness();
      char out[64];
      printf(out,"%f",val);
      AfxMessageBox(out);
      m_agent_manager->unPause();
   }*/
/*
   m_meta_graph->loadGraphAgent();
   m_meta_graph->getLineLayer(0,0).setEditable(true);
   */
/*
   for (int run = 0; run < 3; run++) {
      int seed = run + 2;
      int return_time = 600;
      //
      if (m_evoecomorph) {
         delete m_evoecomorph;
         m_evoecomorph = NULL;
      }
      pafsrand(seed);
      m_evoecomorph = new Evoecomorph;
      delete m_meta_graph;
      m_meta_graph = new MetaGraph;
      m_evoecomorph->init( *m_meta_graph, return_time );
      for (int genset = 0; genset < 10; genset++) {
         if (genset != 0) {
            delete m_meta_graph;
            m_meta_graph = new MetaGraph;
            m_evoecomorph->next( *m_meta_graph );
            //m_evoecomorph->blank( *m_meta_graph ); // this was set to blank, I don't know why
         }
         for (int i = 0; i < 500; i++) {
            char blah2[128];
            for (int j = 0; j < 300 + (return_time * 3) / 2; j++) { // moved down from 600 to 300 in line with changes to evoecomorph loading
               m_evoecomorph->step(*m_meta_graph);
            }
            m_evoecomorph->evaluate(*m_meta_graph);
            sprintf(blah2, "seed %d gen %d fit %f", seed, i + genset * 500, m_evoecomorph->m_fitness);
            SetTitle(blah2);
            delete m_meta_graph;
            if (i < 499) {
               m_meta_graph = new MetaGraph;
               m_evoecomorph->next( *m_meta_graph );
            }
         }
         m_meta_graph = new MetaGraph;
         m_evoecomorph->best( *m_meta_graph );
         for (int k = 0; k < 300 + (return_time * 3) / 2; k++) {  // moved down from 600 to 300
            m_evoecomorph->step(*m_meta_graph);
         }
         m_evoecomorph->evaluate(*m_meta_graph);
         char blah3[128];
         char blah4[128];
         sprintf(blah3, "dcc%d-seed%d-gen%d-noexit.graph", return_time, seed, i + genset * 500);
         sprintf(blah4, "dcc%d-seed%d-gen%d-noexit.txt", return_time, seed, i + genset * 500);
         ofstream summary(blah4);
         char blah[256];
         sprintf(blah, "Bad rooms %d, bad agents %d, avg diff %f, fitness %f",
            m_evoecomorph->m_bad_rooms, m_evoecomorph->m_bad_agents, m_evoecomorph->m_avg_diff, m_evoecomorph->m_fitness);
         summary << blah << endl;
         m_meta_graph->write(blah3, METAGRAPH_VERSION);
      }
   }

   SetUpdateFlag(QGraphDoc::NEW_DATA);
   SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA );

   char blah[256];
   sprintf(blah, "Bad rooms %d, bad agents %d, avg diff %f, fitness %f",
      m_evoecomorph->m_bad_rooms, m_evoecomorph->m_bad_agents, m_evoecomorph->m_avg_diff, m_evoecomorph->m_fitness);

   AfxMessageBox(blah);
*/
   /*
   static int x = 0;
   static bool widdywoo = false;

   if (m_meta_graph && m_meta_graph->getState() & MetaGraph::GRAPH) {
      // cross fingers and go for it!
      if (!widdywoo) {
         x = m_meta_graph->addLineDynamic(Line(Point2f(0.0,0.0),Point2f(10.0,10.0)));
         widdywoo = true;
      }
      else {
         m_meta_graph->removeLineDynamic(x);
         widdywoo = false;
      }

      m_meta_graph->dynamicSparkGraph2();
   }
   */
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnAddColumn() 
{
   CRenameObjectDlg dlg(tr("Column"));  // using the constructor without a column name sets rename column dialog to insert column name mode
   bool success = false;
   while (dlg.exec() == QDialog::Accepted && !success) {
      if (dlg.m_object_name.isEmpty()) {
		  QMessageBox::warning(this, tr("Warning"), tr("Column name cannot be empty"), QMessageBox::Ok, QMessageBox::Ok);
      }
      else {
         AttributeTable& tab = m_meta_graph->getAttributeTable();
         bool found = false;
         for (int i = 0; i < tab.getColumnCount(); i++) {
            if (tab.getColumnName(i) == dlg.m_object_name.toStdString()) {
				QMessageBox::warning(this, tr("Notice"), tr("Sorry, another column already has this name, please choose a unique column name"), QMessageBox::Ok, QMessageBox::Ok);
               found = true;
               break;
            }
         }
         if (!found) {
            success = true;
            break;
         }
      }
   }
   if (success) {
      int col = m_meta_graph->addAttribute(dlg.m_object_name.toStdString());
      m_meta_graph->setDisplayedAttribute(col);
      SetUpdateFlag(QGraphDoc::NEW_DATA);
      // Tell the views to update their menus
      SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_COLUMN );
   }
}

void QGraphDoc::OnRenameColumn() 
{
   AttributeTable *tab = &(m_meta_graph->getAttributeTable());
   int col = m_meta_graph->getDisplayedAttribute();
   // -1 is reference number, -2 is displaying nothing (-2 shouldn't happen but is)
   if (col == -1 || col == -2 || m_meta_graph->isAttributeLocked(col)) {
	   QMessageBox::warning(this, tr("Warning"), tr("Cannot rename locked column"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   int newcol = RenameColumn(tab,col);
   if (newcol != -1) {
      m_meta_graph->setDisplayedAttribute(newcol);
      SetUpdateFlag(QGraphDoc::NEW_DATA);
      SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_COLUMN );
   }
}

int QGraphDoc::RenameColumn(AttributeTable *tab, int col)
{
   QString colname = QString(tab->getColumnName(col).c_str());
   CRenameObjectDlg dlg("Column",colname);  // using the column name sets the dialog to replace column name mode
   bool success = false;
   while (dlg.exec() == QDialog::Accepted && !success && dlg.m_object_name != colname) {
      int newcol = tab->renameColumn(col,dlg.m_object_name.toStdString());
      if (newcol == -1) {
		  QMessageBox::warning(this, tr("Notice"), tr("Sorry, another column already has this name, please choose a unique column name"), QMessageBox::Ok, QMessageBox::Ok);
      }
      else {
         return newcol;
      }
   }
   return -1;
}

void QGraphDoc::OnColumnProperties() 
{
   AttributeTable *tab = &(m_meta_graph->getAttributeTable());
   int col = m_meta_graph->getDisplayedAttribute();

   CColumnPropertiesDlg dlg(tab,col);
   
   dlg.exec();
}

///////////////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnUpdateColumn() 
{
   int col = m_meta_graph->getDisplayedAttribute();
   // -1 is reference number, -2 is displaying nothing (-2 shouldn't happen but is)
   if (col == -1 || col == -2 || m_meta_graph->isAttributeLocked(col)) {
	   QMessageBox::warning(this, tr("Warning"), tr("Cannot edit locked column"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   PointMap *pointmap = NULL;
   ShapeMap *shapemap = NULL;
   int vc = m_meta_graph->getViewClass();
   if (vc & MetaGraph::VIEWVGA) {
      pointmap = &(m_meta_graph->getDisplayedPointMap());
   }
   else if (vc & MetaGraph::VIEWAXIAL) {
      shapemap = &(m_meta_graph->getDisplayedShapeGraph());
   }
   else if (vc & MetaGraph::VIEWDATA) {
      shapemap = &(m_meta_graph->getDisplayedDataMap());
   }

   if (ReplaceColumnContents(pointmap,shapemap,col)) {
      m_meta_graph->setDisplayedAttribute(col);
      SetUpdateFlag(QGraphDoc::NEW_DATA);
      SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_DATA );
   }
}

// Either shapemap or pointmap should be NULL:
bool QGraphDoc::ReplaceColumnContents(PointMap *pointmap, ShapeMap *shapemap, int col)
{
   SalaObj program_context;
   SalaGrf graph;
   if (pointmap != NULL) {
      graph.map.point = pointmap;
      program_context = SalaObj(SalaObj::S_POINTMAPOBJ, graph);
   }
   else if (shapemap != NULL) {
      SalaGrf graph;
      graph.map.shape = shapemap;
      program_context = SalaObj(SalaObj::S_SHAPEMAPOBJ, graph);
   }
   else {
	   QMessageBox::warning(this, tr("Warning"), tr("Replace column contents requires either a shape map or visibility graph"), QMessageBox::Ok, QMessageBox::Ok);
      return false;
   }

   AttributeTable *table = program_context.getTable();

   // insert dialog is a misnomer now!
   CInsertColumnDlg dlg(table,col);  // Using a column number sets it to use the replace text rather than select text
   
   bool error = true;
   while (error && QDialog::Accepted == dlg.exec()) {
      error = false;
      size_t n = dlg.m_formula_text.length();
      char *text = new char[n+1];
      if (n == 0) {
		  QMessageBox::warning(this, tr("Notice"), tr("Please enter a formula"), QMessageBox::Ok, QMessageBox::Ok);
         error = true;
      }
      else {
         strcpy(text,dlg.m_formula_text.c_str());
         istringstream stream(text);
         SalaProgram proggy(program_context);
         if (!proggy.parse(stream)) {
            QString msg = QString("There was an error parsing your formula:\n\n") + 
               proggy.getLastErrorMessage().c_str();
			QMessageBox::warning(this, tr("Warning"), msg, QMessageBox::Ok, QMessageBox::Ok);
            error = true;
         }
         else {
            // just check you really are viewing the layers:
            bool retvar;
            if (dlg.m_selection_only) {
               retvar = proggy.runupdate(col,pointmap ? pointmap->getSelSet() : shapemap->getSelSet());
            }
            else {
               retvar = proggy.runupdate(col);
            }
            if (!retvar) {
               QString msg = QString("There was an error running your formula:\n\n") + 
                  proggy.getLastErrorMessage().c_str();
               QMessageBox::warning(this, tr("Warning"), msg, QMessageBox::Ok, QMessageBox::Ok);
               error = true;
            }
         }
      }
      if (!error) {
         table->setColumnFormula(col,text);
      }
      delete [] text;
   }
   return !error;
}

void QGraphDoc::OnEditQuery() 
{
   PointMap *pointmap = NULL;
   ShapeMap *shapemap = NULL;
   int vc = m_meta_graph->getViewClass();
   if (vc & MetaGraph::VIEWVGA) {
      pointmap = &(m_meta_graph->getDisplayedPointMap());
   }
   else if (vc & MetaGraph::VIEWAXIAL) {
      shapemap = &(m_meta_graph->getDisplayedShapeGraph());
   }
   else if (vc & MetaGraph::VIEWDATA) {
      shapemap = &(m_meta_graph->getDisplayedDataMap());
   }

   if (SelectByQuery(pointmap,shapemap)) {
      SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_DATA );
   }
}

// Either shapemap or pointmap should be NULL:
bool QGraphDoc::SelectByQuery(PointMap *pointmap, ShapeMap *shapemap)
{
   SalaObj program_context;
   SalaGrf graph;
   if (pointmap != NULL) {
      graph.map.point = pointmap;
      program_context = SalaObj(SalaObj::S_POINTMAPOBJ, graph);
   }
   else if (shapemap != NULL) {
      SalaGrf graph;
      graph.map.shape = shapemap;
      program_context = SalaObj(SalaObj::S_SHAPEMAPOBJ, graph);
   }
   else {
	   QMessageBox::warning(this, tr("Warning"), tr("Replace column contents requires either a shape map or visibility graph"), QMessageBox::Ok, QMessageBox::Ok);
      return false;
   }

   AttributeTable *table = program_context.getTable();
   // insert dialog is a misnomer now!
   CInsertColumnDlg dlg(table,-1);  // -1 sets it to use the select text rather than replace text
   bool error = true;
   while (error && QDialog::Accepted == dlg.exec()) {
      error = false;
      std::string multibytetext(((MainWindow*)m_mainFrame)->m_formula_cache.toStdString());
      char *text = new char[multibytetext.length()+1];
      strcpy(text,multibytetext.c_str());
      istringstream stream(text);
      SalaProgram proggy(program_context);
      if (!proggy.parse(stream)) {
         QString msg = QString("There was an error parsing your formula:\n") + 
            QString(proggy.getLastErrorMessage().c_str());
         QMessageBox::warning(this, tr("depthmapX"), msg, QMessageBox::Ok, QMessageBox::Ok);
         error = true;
      }
      else {
         // just check you really are viewing the layers:
         bool retvar;
         std::vector<int> selset;
         if (dlg.m_selection_only) {
            retvar = proggy.runselect(selset,pointmap ? pointmap->getSelSet() : shapemap->getSelSet());
         }
         else {
            retvar = proggy.runselect(selset);
         }
         if (!retvar) {
            QString msg = QString("There was an error running your formula:\n") + 
               QString(proggy.getLastErrorMessage().c_str());
            QMessageBox::warning(this, tr("depthmapX"), msg, QMessageBox::Ok, QMessageBox::Ok);
            error = true;
         }
         else {
            // make the selection using the selset:
            if (pointmap) {
               pointmap->setCurSel(selset);
            }
            else {
               // note, shape maps have been working with rowids directly:
               shapemap->setCurSelDirect(selset);
            }
         }
      }
      delete [] text;
      if (!error) {
         return true;
      }
   }
   return false;
}

void QGraphDoc::OnEditSelectToLayer()
{
   if ((m_meta_graph->getViewClass() & (MetaGraph::VIEWAXIAL|MetaGraph::VIEWDATA))
      && m_meta_graph->isSelected()) {

      CRenameObjectDlg dlg("Layer"); // note, without specifying existing layer name, this defaults to "New layer" behaviour
      if (QDialog::Accepted == dlg.exec()) {

         auto layer_name = dlg.m_object_name.toStdString();
         if (layer_name.empty()) {
            layer_name = "Untitled";
         }

         bool retvar = false;
         if (m_meta_graph->getViewClass() & (MetaGraph::VIEWAXIAL)) {
            retvar = m_meta_graph->getDisplayedShapeGraph().selectionToLayer(layer_name);
         }
         else {
            retvar = m_meta_graph->getDisplayedDataMap().selectionToLayer(layer_name);
         }
         if (retvar) {
            SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
            SetUpdateFlag(QGraphDoc::NEW_TABLE);
         }
         else {
			 QMessageBox::warning(this, tr("Warning"), tr("Couldn't create new layer.\nPlease note there is a limit of 64 layers per map."), QMessageBox::Ok, QMessageBox::Ok);
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////

void QGraphDoc::OnRemoveColumn() 
{
  // just check you really are viewing the layers:
   int col = m_meta_graph->getDisplayedAttribute();
   // -1 is reference number, -2 is displaying nothing (-2 shouldn't happen but is)
   if (col == -1 || col == -2 || m_meta_graph->isAttributeLocked(col)) {
	   QMessageBox::warning(this, tr("Warning"), tr("Cannot remove locked column"), QMessageBox::Ok, QMessageBox::Ok);
   }
   else if (QMessageBox::Yes == QMessageBox::question(this, tr("depthmapX"), tr("Are you sure you want to delete the currently displayed column?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No)) {
      // note this -1 simply means shift back one
      m_meta_graph->setDisplayedAttribute(col-1);
      m_meta_graph->removeAttribute(col);
      SetUpdateFlag(QGraphDoc::NEW_DATA);
      SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_COLUMN );
   }
}

////////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnFileProperties() 
{
   if (!m_meta_graph) {
	   QMessageBox::warning(this, tr("Warning"), tr("No graph"), QMessageBox::Ok, QMessageBox::Ok);
   }
   else {
      CFilePropertiesDlg dlg;
      // editables
	   dlg.m_title = QString(m_meta_graph->getTitle().c_str());
	   dlg.m_location = QString(m_meta_graph->getLocation().c_str());
	   dlg.m_description = QString(m_meta_graph->getDescription().c_str());
      // non-editables
	   dlg.m_author = QString(m_meta_graph->getPerson().c_str());
	   dlg.m_create_date = QString(m_meta_graph->getDate().c_str());
	   dlg.m_create_program = QString(m_meta_graph->getProgram().c_str());
	   dlg.m_organization = QString(m_meta_graph->getOrganization().c_str());
      if (m_meta_graph && m_meta_graph->getVersion() != -1) {
         QString info;
         info.sprintf("%d", m_meta_graph->getVersion());
         dlg.m_file_version = info;
      }
      else {
         dlg.m_file_version = tr("<Unsaved>");
      }
      if (QDialog::Accepted == dlg.exec()) {
         m_meta_graph->setTitle(dlg.m_title.toStdString());
         m_meta_graph->setLocation(dlg.m_location.toStdString());
         m_meta_graph->setDescription(dlg.m_description.toStdString());
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

// a bit of testing

void QGraphDoc::OnMagiMif() 
{
   if (m_meta_graph && !m_communicator) {
      if (m_meta_graph->getState() & MetaGraph::SHAPEGRAPHS) {

         QFilePath path(m_opened_name);
         QString defaultname = path.m_path + (path.m_name.isEmpty() ? "" : path.m_name) + tr("_polys");
		 
		 QFileDialog::Options options = 0;
		 QString outfile = QFileDialog::getOpenFileName(
								   0, tr("Save As"),
								   defaultname,
								   tr("MapInfo file (*.mif)"),
								   0,
								   options);
         
		 if(!outfile.isEmpty()){
            QFilePath filepath(outfile);
            QString ext = filepath.m_ext;         
            if (ext == QString("MIF")) 
            {
               int thedot = outfile.indexOf('.');
               QString outfile2 = outfile.left(thedot+1) + tr("mid");
               
               ofstream miffile(outfile.toLatin1());
               if (miffile.fail() || miffile.bad()) {
                  QMessageBox::warning(this, tr("Warning"), tr("Sorry, unable to open file for export"), QMessageBox::Ok, QMessageBox::Ok);
               }
               
               ofstream midfile(outfile2.toLatin1());
               if (midfile.fail() || midfile.bad()) {
	               QMessageBox::warning(this, tr("Warning"), tr("Sorry, unable to open associated .mid file for export"), QMessageBox::Ok, QMessageBox::Ok);
               }             
               m_meta_graph->getDisplayedShapeGraph().outputMifPolygons(miffile,midfile);
            }
         }
      }
   }
}

void QGraphDoc::OnBinDistances() 
{
   m_meta_graph->getDisplayedPointMap().binMap(NULL);
}

void QGraphDoc::OnShowBinDistances() 
{
   std::set<int> a = m_meta_graph->getDisplayedPointMap().getSelSet();
   Point& p = m_meta_graph->getDisplayedPointMap().getPoint(*a.begin());
   QString all;
   for (int i = 0; i < 32; i++) {
      QString blah = QString(tr("%2d: %f\n")).arg(i).arg(p.getBinDistance(i));
      all += blah;
   }
   QMessageBox::information(this, tr("BinDistances"), all, QMessageBox::Ok, QMessageBox::Ok);
}

void QGraphDoc::OnViewShowGrid() 
{
   if (m_meta_graph->m_showgrid) {
      m_meta_graph->m_showgrid = false;
   }
   else {
      m_meta_graph->m_showgrid = true;
   }
   SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_DEPTHMAPVIEW_SETUP);
}

void QGraphDoc::OnViewShowText() 
{
   if (m_meta_graph->m_showtext) {
      m_meta_graph->m_showtext = false;
   }
   else {
      m_meta_graph->m_showtext = true;
   }
   SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_DEPTHMAPVIEW_SETUP);
}

//#include "AttributeSummary.h"

void QGraphDoc::OnViewSummary() 
{
   CAttributeSummary dlg(this);
   dlg.exec();
}

////////////////////////////////////////////////////////////////////////////////////////////

// Red button tests the rapid agent program:

void QGraphDoc::OnRedButton() 
{
/*   AfxMessageBox(tr("Disabled: use main through vision calculator (with vga analysis) instead"));
   // m_meta_graph->analyseThruVision();
   //
   SetUpdateFlag(QGraphDoc::NEW_DATA);
   SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_DATA );*/
}

void QGraphDoc::OnToolsPointConvShapeMap() 
{
   //CWaitCursor wait;
   m_meta_graph->getDisplayedPointMap().mergeFromShapeMap(m_meta_graph->getDisplayedDataMap());
   m_meta_graph->setViewClass(MetaGraph::SHOWVGATOP);
   SetUpdateFlag(QGraphDoc::NEW_TABLE);
   SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
}

// this is unlink from a set of points!
void QGraphDoc::OnToolsAxialConvShapeMap() 
{
   if (m_meta_graph->getDataMaps().getMapCount() == 0) {
	   QMessageBox::warning(this, tr("Warning"), tr("No data source layers for unlink points"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }

   std::vector<std::string> names;
   for (size_t i = 0; i < m_meta_graph->getDataMaps().getMapCount(); i++) {
      names.push_back(std::string("Data Maps: ") + m_meta_graph->getDataMaps().getMap(i).getName());
   }

   // choose shape map...
   CLayerChooserDlg dlg(names);
   dlg.m_text = tr("Please select source layer for unlink points");

   if (dlg.exec()) {
      //CWaitCursor wait;
      m_meta_graph->getDisplayedShapeGraph().unlinkFromShapeMap(m_meta_graph->getDataMaps().getMap(dlg.m_layer));
      m_meta_graph->setViewClass(MetaGraph::SHOWAXIALTOP);
      SetUpdateFlag(QGraphDoc::NEW_TABLE);
      SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH, NEW_DATA);
   }
}

void QGraphDoc::OnToolsLineLoadUnlinks() 
{
   // Get file from user 
   QFileDialog::Options options = 0;
   QString outfile = QFileDialog::getOpenFileName(
                               0, tr("Import Unlink IDs"),
                               "",
                               tr("Text files (*.txt)\nAll files (*.*)"),
                               0,
                               options);
   if(outfile.isEmpty()) return;

   ifstream stream(outfile.toLatin1());
   if (stream.fail()) {
	  QMessageBox::warning(this, tr("Warning"), tr("There was an error opening the file.\nPlease check the file is not already open"), QMessageBox::Ok, QMessageBox::Ok);
      return;
   }
   // Check to see if they want to use another key column for the unlink:
   CAttributeChooserDlg dlg(m_meta_graph->getDisplayedShapeGraph().getAttributeTable());
   dlg.m_text = tr("Please select the attribute the file uses as the key for the unlinks");
   if (dlg.exec() == QDialog::Accepted) {
      // Run the process
      if (!m_meta_graph->getDisplayedShapeGraph().unlinkShapeSet(stream,dlg.m_attribute)) {
          QMessageBox::warning(this, tr("Warning"), tr("There was an error reading the unlink file.\ndepthmapX is expecting a tab delimited set of unlink IDs"), QMessageBox::Ok, QMessageBox::Ok);
          return;
      }
      m_meta_graph->setViewClass(MetaGraph::SHOWAXIALTOP);
      SetUpdateFlag(QGraphDoc::NEW_DATA);
      SetRedrawFlag(VIEW_ALL,REDRAW_GRAPH,NEW_DATA);
   }
}

void QGraphDoc::OnConvertMapShapes()
{
   if (m_meta_graph && m_meta_graph->viewingShapes()) {
      CConvertShapesDlg dlg;
      if (QDialog::Accepted == dlg.exec()) {
         int viewclass = m_meta_graph->getViewClass();
         if (viewclass & MetaGraph::VIEWDATA) {
            m_meta_graph->getDisplayedDataMap().convertPointsToPolys(dlg.m_radius,dlg.m_selected_only);
         }
         else if (viewclass & MetaGraph::VIEWAXIAL) {
            m_meta_graph->getDisplayedDataMap().convertPointsToPolys(dlg.m_radius,dlg.m_selected_only);
         }
         else {
			 QMessageBox::warning(this, tr("Warning"), tr("Cannot perform conversion on this map type"), QMessageBox::Ok, QMessageBox::Ok);
         }
      }
   }
}

// Point depth either lines, axials or segments: 

// Deprecated, now in CViewSelector

bool QGraphDoc::ViewHandler(int nCode, void *pExtra, int viewing, int layer)
{
/*
   if (nCode == CN_COMMAND) {
      if (layer != -1) {
         if (!m_meta_graph->setCurrentLayerRef(layer)) {
            // Layer doesn't exist for some reason...
            return false;
         }
         SetTitle(m_base_title + " - " + m_meta_graph->getCurrentLayer().getLayerName().c_str());
      }
      else {
         SetTitle(m_base_title + " - Attributes");
      }
      if (m_meta_graph->getViewing() != viewing) {
         m_meta_graph->setViewing(viewing, *m_meta_graph);
      }
      SetRedrawFlag(VIEW_ALL, true );
      SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH );
   }
   else if (nCode == CN_UPDATE_COMMAND_UI) {
      (()//CCmdUI*)pExtra)->Enable(TRUE);
      if (m_meta_graph->getViewing() == viewing && 
          (layer == -1 || m_meta_graph->getCurrentLayerRef() == layer)) {
         (()//CCmdUI*)pExtra)->SetCheck(1);
      }
      else {
         (()//CCmdUI*)pExtra)->SetCheck(0);
      }
   }
*/
   return true;
}

bool QGraphDoc::CheckMemory(const QString& filename) 
{
/*   // Check there's enough memory to hold the graph (based on benchmarks)
   MEMORYSTATUS memstat;
   memstat.dwLength = sizeof(MEMORYSTATUS);
   GlobalMemoryStatus(&memstat);
   double avail = double(memstat.dwAvailPhys) / double(1024 * 1024);

   double min_required = double(m_meta_graph->getDisplayedPointMap().getPointCount()) * 0.0050;
   double absolute_min_req = double(m_meta_graph->getDisplayedPointMap().getPointCount()) * 0.0025;

   if (!filename.IsEmpty()) {
      ifstream test(filename, ios::binary);
      if (test.fail()) {
         AfxMessageBox(QString("Couldn't open ") + filename);
         if (test.is_open()) {
            test.close();
         }
         return false;
      }
      char header[3];
      test.read( (char *) &header, 3 );
      if (header[0] != 'g' || header[1] != 'r' || header[2] != 'f') {
         AfxMessageBox(tr("Sorry: this is not a depthmapX graph file"));
         test.close();
         return false;
      }
      int version;
      test.read( (char *) &version, sizeof(version));
      test.seekg(0,ios::end);
      if (version < VERSION_NGRAPH_INTROD) {
         // Should be able to fit old graphs into about the same memory:
         min_required = 1.1 * double(test.tellg()) / double(1024 * 1024);
      }
      else {
         // Don't know why, but the current version appears double the file size when in memory:
         min_required = 1.1 * double(test.tellg()) / double(1024 * 1024); // .... did use to be 2.2
      }
      test.close();
      if (version < VERSION_NGRAPH_INTROD) {
         if (IDYES != AfxMessageBox(tr("Warning, this graph will require conversion, which may take a few minutes.\nDo you want to continue?"), MB_YESNO)) {
            return false;
         }
      }
   }

   if (avail < min_required) {
      QString str;
      str.Format(tr("%.1f MB available, %.1f MB advised"), avail, min_required );
      if ( IDYES != AfxMessageBox( QString("According to benchmark stats, there may not be enough memory to process this graph.\n") +
                                   tr("(") + str + tr(")\n") +
                                   tr("The analysis may take much longer than usual.  Do you still want to continue?"), MB_YESNO ) ) {
         return false;
      }
   }
*/
   return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QT_END_NAMESPACE

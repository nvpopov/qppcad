#include <iostream>
#include <QApplication>
#include <QStyleFactory>
#include <QFontDatabase>
#include <qppcad/ui/main_window.hpp>
#include <qppcad/app_state.hpp>
#include <QTextStream>
#include <QCommandLineParser>

using namespace qpp;
using namespace qpp::cad;

int main (int argc, char **argv) {

  std::ios_base::sync_with_stdio(false);

  QCoreApplication::setOrganizationName("igc");
  QCoreApplication::setOrganizationDomain("igc.irk.ru");
  QCoreApplication::setApplicationName("qppcad");
  QApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("qpp::cad");
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("file", "The file to open.");

  QCommandLineOption target_fmt_option(QStringList() << "f" << "format",
                                       QCoreApplication::translate("main", "Force file format"),
                                       QCoreApplication::translate("main", "file_format"));
  parser.addOption(target_fmt_option);

  parser.process(QCoreApplication::arguments());
  const QStringList args = parser.positionalArguments();


  app_state_t::init_inst();

  app_state_t *astate = app_state_t::get_inst();
  astate->init_managers();
  astate->ws_manager->init_ws_item_bhv_mgr();
  astate->ws_manager->init_default();
  astate->load_settings();

  if (!args.empty()) {

      std::string file_name = args[0].toStdString();
      std::string file_format = "";

      if (parser.isSet(target_fmt_option))
        file_format = parser.value(target_fmt_option).toStdString();

      astate->ws_manager->load_from_file_autodeduce(file_name, file_format);

    }

  QSurfaceFormat format;
  format.setDepthBufferSize(24);

  //format.setStencilBufferSize(8);
  format.setSamples(astate->m_num_samples);
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  qApp->setStyle(QStyleFactory::create("Fusion"));
  astate->init_styles();
  qApp->setPalette(astate->m_app_palette);

  QFontDatabase::addApplicationFont("://fonts/Open_Sans/OpenSans-Light.ttf");
  QFontDatabase::addApplicationFont("://fonts/Open_Sans/OpenSans-Bold.ttf");

  astate->m_font_name = "OpenSans";

  QFont font = QFont(astate->m_font_name, 12, 1);
  QIcon icon("://images/icon.svg");
  //QFont defaultFont = QApplication::font();
  font.setPointSize(font.pointSize());
  //qApp->setFont(defaultFont);
  app.setWindowIcon(icon);
  app.setFont(font);

  QFile file("://style.qss");
  file.open(QFile::ReadOnly);
  QString style_sheet = QLatin1String(file.readAll());
  app.setStyleSheet(style_sheet);

  main_window w;
  w.rebuild_recent_files_menu();
  w.showMaximized();
  int ret_code = app.exec();

  app_state_t::get_inst()->save_settings();

  return ret_code;

}

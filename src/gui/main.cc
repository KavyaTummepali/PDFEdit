/** @file
 Main function
*/
#include "config.h"
#include "util.h"
#include "version.h"
#include "pdfeditwindow.h"
#include "settings.h"
#include <stdlib.h>
#include <qtranslator.h>
#include <qdir.h>
#include <iostream>

using namespace std;

/** Option handler function*/
typedef void optHandlerFn(void);

/** Option handler function pointer*/
typedef optHandlerFn *optHandler;

/** Option handler map*/
typedef QMap<QString, optHandler> OptionMap;

/** Option help map*/
typedef QMap<QString, QString> OptionHelp;

/** Name of the program (argv[0]) */
QString binName;
/** Option help texts */
OptionHelp optHelp;
/** Option handlers */
OptionMap optMap;

/** One object for application, holding all global settings.
 Should be thread-safe. This instance is used from other files */
Settings *globalSettings;

/** delete settings object (and save settings)
 This function is called at application exit
 */
void saveSettings(void) {
 delete globalSettings;//this causes settings to be saved to disk
}

/** handle --version parameter */
void handleVersion(){
  cout << VERSION << endl;
  exit(0);
}

/** handle --help parameter */
void handleHelp(){
  cout << APP_NAME << " " << VERSION << endl;
  cout << QObject::tr("Usage: ") << binName << QObject::tr(" [option(s)] [files(s)]") << endl;
  cout << QObject::tr("Options: ") << endl;
  QValueList<QString> opt=optHelp.keys();
  for (QValueList<QString>::Iterator it=opt.begin();it!=opt.end();++it) {
   cout << " ";
   cout.width(16);		//width of option name
   cout.flags(ios::left);
   cout << *it << optHelp[*it] << endl;
  }
  exit(0);
}

/** Register function to handle options
 @param param Name of option (case sensitive)
 @param h Function to handle this option
 @param help Brief one-line help about this option
 */
void optionHandler(const QString &param, optHandler h,const QString &help="") {
 optMap[param]=h;
 optHelp[param]=help;
}

/** Adds option to option list. Some options (help) are processed immediately
 @param opt Commandline option to check
 @return true if option is valid, false otherwise
 */
bool handleOption(const QString &param) {
 if (!optMap.contains(param)) return false;
 optMap[param]();
 return true;
}

/** main - load settings and launches a main window */
int main(int argc, char *argv[]){
 QApplication app(argc, argv);
 QStringList params;
 QString param;
 optionHandler("--help",handleHelp,QObject::tr("Print help and exit"));
 optionHandler("--version",handleVersion,QObject::tr("Print version and exit"));
 binName=app.argv()[0];
 for (int i=1;i<app.argc();i++) {
  param=app.argv()[i];
  if (param.startsWith("-")) { //option
   if (!handleOption(param)) fatalError(QObject::tr("Invalid commandline option : ")+param);
  } else {
   params+=param;
  }
 }
 //Translation support
 QTranslator translator;
 QString lang=QString("pdfedit_")+getenv("LANG");
 if (!translator.load(lang,QString(DATA_PATH)+"/lang")) { 
  if (!translator.load(lang,QDir::home().path()+"/"+CONFIG_DIR+"/lang")) { 
   #ifdef TESTING
   //look in current directory for testing version
   if (!translator.load(lang,"./lang")) { 
   #endif
    printDbg(debug::DBG_WARN,"Translation file " << lang << "not found");
   #ifdef TESTING
   }
   #endif
  }
 }
 app.installTranslator(&translator);

 globalSettings=new Settings();
 globalSettings->setName("settings");
 atexit(saveSettings);
 int nFiles=params.size();
 if (nFiles) { //open files from cmdline
  for (QStringList::Iterator it=params.begin();it!=params.end();++it) {
   //TODO: more than one file = segfault
   printDbg(debug::DBG_INFO,"Opening parameter: " << *it)
   createNewEditorWindow(*it);
  }
 } else { //no parameters
  createNewEditorWindow();
 }
 QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
 return app.exec();
}

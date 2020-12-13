#include <QProcess>
#include <QDebug>
#include <QApplication>
#include <QInputDialog>
#include <DInputDialog>
#include "utilities.h"

QString findDVIWorkFile(const QDir &dviTmpDir)
{
  auto tmpDirContent = dviTmpDir.entryList();
  if(tmpDirContent.size() == 0)
  {
    return "";
  }
  tmpDirContent.removeFirst(); // "."
  tmpDirContent.removeFirst(); // and ".."
  QString concatePath;
  QFileInfo verifyIsDir;
  QDir ventoyDir;
  foreach(auto &&i, tmpDirContent)
  {
    concatePath = dviTmpDir.path() + "/" + i;
    verifyIsDir.setFile(concatePath);
    if(verifyIsDir.isDir())
    {
      ventoyDir.setPath(verifyIsDir.absoluteFilePath());
    }
  }

  QFileInfo mainScriptInfo(ventoyDir, "tool/ventoy_lib.sh"),
            helpScriptInfo(ventoyDir, "Ventoy2Disk.sh");
  if(mainScriptInfo.exists() && helpScriptInfo.exists())
  {
    if(!helpScriptInfo.isExecutable())
    {
      QFile::setPermissions(helpScriptInfo.path(), QFile::ExeUser | QFile::ReadUser);
    }
    QProcess runTest;
    runTest.setWorkingDirectory(ventoyDir.absolutePath());
    runTest.start("Ventoy2Disk.sh"); // Decompress tools first then we run test
    runTest.waitForFinished();
    qInfo() << runTest.readAll().toStdString().data();
    runTest.start("sh");
    runTest.waitForStarted();
    runTest.write(". tool/ventoy_lib.sh \n");
    runTest.write("check_tool_work_ok \n");
    runTest.write("exit\n");
    qInfo() << runTest.readAll().toStdString().data();
    runTest.waitForFinished(100);
    if(!runTest.exitCode())
    {
      return ventoyDir.absolutePath();
    }
    qInfo() << runTest.exitCode() << runTest.readAllStandardError();
  }

  return "";
}

SemanticVersion::SemanticVersion(QString aVer)
{
  auto l = aVer.split('.');
  if(l.size() < 3)
  {
    major = minor = patch = 0;
    return;
  }
  major = l[0].toUInt();
  minor = l[1].toUInt();
  patch = l[2].toUInt();
}

SemanticVersion::SemanticVersion(uint aMajor, uint aMinor, uint aPatch)
{
  major = aMajor;
  minor = aMinor;
  patch = aPatch;
}

SemanticVersion::SemanticVersion()
{
  major = minor = patch = 0;
}

void rerunOnNonRoot()
{
  if(qgetenv("USER") != "root")
  {

    Dtk::Widget::DInputDialog input;
    input.resize(300, 300);
    input.setWindowTitle(QObject::tr("Run As Root"));
    input.setLabelText(QObject::tr("This application must be run as root user.\n"
                                   "Provide password to continue."));
    input.setTextEchoMode(QLineEdit::Password);
    input.exec();
    QString password = input.textValue();
    QProcess rerun;
    rerun.startDetached("bash", QStringList() << "-c"
                << QString("echo %1 | sudo -S %2 %3")
                .arg(password)
                .arg(QCoreApplication::applicationFilePath())
                .arg(QCoreApplication::applicationPid()));
    //rerun.waitForFinished(1000);
    exit(1);
  }
}

void enableCheckBox(bool criteria, QCheckBox *w)
{
  //qInfo() << criteria;
  w->setEnabled(criteria);
  if(!criteria)
  {
    w->setChecked(criteria);
  }
}

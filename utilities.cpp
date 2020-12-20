#include <QProcess>
#include <QDebug>
#include <QApplication>
#include <DDialog>
#include <DLineEdit>
#include "utilities.h"

DWIDGET_USE_NAMESPACE

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
    runTest.start("sh");
    runTest.waitForStarted();
    runTest.write(". tool/ventoy_lib.sh \n");
    runTest.write("check_tool_work_ok \n");
    runTest.write("exit\n");
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

    Dtk::Widget::DDialog input;
    Dtk::Widget::DLineEdit *edit = new Dtk::Widget::DLineEdit(&input);
    edit->setEchoMode(QLineEdit::Password);
    input.setTitle(QObject::tr("Run As Root"));
    input.setMessage(QObject::tr("This application must be run as root user.\n"
                                   "Provide password to continue.\n")); // An empty line to separate contents
    input.addContent(edit);
    input.addButton(QObject::tr("Authenticate"), true, DDialog::ButtonType::ButtonRecommend);
    input.setIcon(QIcon::fromTheme("dialog-information"));
    input.setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    edit->lineEdit()->setFocus(Qt::FocusReason::TabFocusReason); // FIXME: The input will not get focused despite my efforts.
    QObject::connect(edit, SIGNAL(returnPressed()), &input, SLOT(accept()));
    //QObject::connect(&input, SIGNAL())

    // The only way to explicitly close the window will return -1, not rejected (0) nor accepted (1)
    if(input.exec() != -1)
    {
      QString password = edit->text();
      QProcess rerun;
      rerun.startDetached("bash", QStringList() << "-c"
                  << QString("echo %1\"\n\n\n\n\n\n\n\n\" | sudo -S %2") // If fails then ENTER ENTER ENTER til sudo ends
                  .arg(password)
                  .arg(QCoreApplication::applicationFilePath()));
    }
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

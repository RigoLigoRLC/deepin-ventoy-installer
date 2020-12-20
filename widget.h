#ifndef WIDGET_H
#define WIDGET_H

#include "utilities.h"

#include <QWidget>
#include <QThread>
#include <QProcess>
#include <QDir>

namespace Ui {
class Widget;
}

class MainWindow;

class Widget : public QWidget
{
    Q_OBJECT

  public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    bool hasOngoingOperation();

    /**
     * @brief forcifully stop all ongoing processes
     */
    void forceStopAll();

  public slots:
    /**
     * @brief clean tmp folder and unarchive the specified archive.
     * @param aPath the target archive path.
     * @param aNoTest if true, do not test if archive works.
     */
    void processVentoyArchive(QString aPath, bool aNoTest);

    /**
     * @brief get the Ventoy version in selected device. Must select a device.
     */
    void getDeviceVentoyVersion();

    /**
     * @brief query the available storage devices and refresh combo box.
     */
    void queryDeviceList();

    /**
     * @brief switch pager to install details page.
     */
    void switchToInstaller();

    /**
     * @brief set the enabled property of UI according to m_archiveVer
     */
    void verifyVentoyFeatures();

    void goBackDropArchive();
  private slots:
    void finishedUnarchive();
    void proceedInstallation();
    void finishedInstallation();
    void readInstallLog();

  private slots:
    void on_chkUseCustomVolLabel_stateChanged(int arg1);

    void on_selectDevice_combo_currentIndexChanged(int index);

  private:
    /**
     * @brief initialize main widget.
     */
    void initWidget();

    /**
     * @brief change dark/light theme, mostly for button icons.
     * @param aDark true for dark theme, false for light theme.
     */
    void changeTheme(bool aDark);

    /**
     * @brief get the Ventoy version in extracted archive. Must have archive extracted.
     */
    void getArchiveVentoyVersion();

    /**
     * @brief install Ventoy to the specified device
     * @param aDevice the target device to install into
     * @param aIsUpdate is going to update rather than install
     * @param aIsForced is going to install forcifully
     * @param aUseGpt is going to use GPT partition table
     * @param aUseSecureBoot is going to use secure boot
     * @param aVolLabel the volume label for the ISO partition
     */
    void installVentoy(const QString &aDevice, bool aIsUpdate, bool aIsForced,
                       bool aUseGpt, bool aUseSecureBoot, const QString &aVolLabel);


  private:
    Ui::Widget *ui;
    QDir tmpDir, ventoyDir;
    QProcess m_unarchiveProcess, m_installProcess;
    SemanticVersion m_archiveVer, m_DeviceVer;
    QAction *actGoDropArchive;
    bool m_noTestVentoy;
};

#endif // WIDGET_H

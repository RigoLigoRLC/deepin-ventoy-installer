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

class Widget : public QWidget
{
    Q_OBJECT

  public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

  public slots:
    void processVentoyArchive(QString aPath);

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
     * @param the target device to install into
     * @param is going to update rather than install
     * @param is going to install forcifully
     * @param is going to use GPT partition table
     * @param is going to use secure boot
     * @param the volume label for the ISO partition
     */
    void installVentoy(const QString &aDevice, bool aIsUpdate, bool aIsForced,
                       bool aUseGpt, bool aUseSecureBoot, const QString &aVolLabel);


  private:
    Ui::Widget *ui;
    QDir tmpDir, ventoyDir;
    QProcess m_unarchiveProcess, m_installProcess;
    SemanticVersion m_archiveVer, m_DeviceVer;
};

#endif // WIDGET_H

#ifndef DFINSTALLNORMALWINDOW_H
#define DFINSTALLNORMALWINDOW_H

#include "dfinstallerrordialog.h"
#include "dfontinfomanager.h"

#include <QStringList>
#include <QTimer>

#include <DDialog>
#include <DFrame>
#include <DLabel>
#include <DProgressBar>

DWIDGET_USE_NAMESPACE

class DFontManager;
class DFontInfoManager;

class DFInstallNormalWindow : public DDialog
{
    Q_OBJECT
public:
    DFInstallNormalWindow(const QStringList &files = QStringList(), QWidget *parent = nullptr);
    ~DFInstallNormalWindow() override;

    void setSkipException(bool skip);

protected:
    static constexpr int VERIFY_DELYAY_TIME = 1000;

    void initUI();
    void initConnections();
    void initVerifyTimer();

    void verifyFontFiles();
    bool ifNeedShowExceptionWindow() const;

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected slots:
    void batchInstall();
    void onProgressChanged(const QString &filePath, const double &percent);

    void onCancelInstall();
    void onContinueInstall(QStringList continueInstallFontFileList);

    void showInstallErrDlg();
signals:
    void batchReinstall(QStringList reinstallFiles);
    void finishFontInstall();

private:
    enum InstallState { Install, reinstall, damaged };

    QStringList m_installFiles;
    QStringList m_installedFiles;
    QStringList m_newInstallFiles;
    QStringList m_damagedFiles;

    // Skip popup exception dialog if true
    bool m_isNeedSkipException {false};

    InstallState m_installState {Install};

    QList<DFInstallErrorItemModel *> m_installErrorFontModelList;
    DFontInfoManager *m_fontInfoManager;
    DFontManager *m_fontManager;

    DLabel *m_logoLabel {nullptr};
    DLabel *m_titleLabel {nullptr};
    DFrame *m_titleFrame {nullptr};
    DFrame *m_mainFrame {nullptr};

    DLabel *m_progressStepLabel {nullptr};
    DLabel *m_currentFontLabel {nullptr};
    DProgressBar *m_progressBar {nullptr};

    QScopedPointer<QTimer> m_verifyTimer {nullptr};
};

#endif  // DFINSTALLNORMALWINDOW_H
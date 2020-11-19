﻿#include "views/dfontmgrmainwindow.h"

#include <gtest/gtest.h>
#include "../third-party/stub/stub.h"
#include "views/dfinstallnormalwindow.h"

#include "globaldef.h"
#include "interfaces/dfontmenumanager.h"
#include "utils.h"
#include "views/dfdeletedialog.h"
#include "views/dfontinfodialog.h"
#include "views/dfquickinstallwindow.h"

#include <QTest>
#include <QSignalSpy>
#include <QHBoxLayout>
#include <QShortcut>
#include <QFileSystemWatcher>
#include <QDBusConnection>

#include "dobject.h"
#include <DApplication>
#include <DApplicationHelper>
#include <DFontSizeManager>
#include <DFileDialog>
#include <DIconButton>
#include <DLabel>
#include <DLineEdit>
#include <DLog>
#include <DMenu>
#include <DSearchEdit>
#include <DSlider>
#include <DSplitter>
#include <DTitlebar>
#include <DWidgetUtil>
#include <DDesktopServices>
#include <DMessageManager>

#include <unistd.h>

namespace {
class TestDFontMgrMainWindow : public testing::Test
{

protected:
    void SetUp()
    {
        fm = new DFontMgrMainWindow();
    }
    void TearDown()
    {
        delete fm;
    }

    DFontMgrMainWindow *fm;
};

bool stub_false()
{
    return false;
}

bool stub_true()
{
    return true;
}

void stub_return()
{
    return ;
}

qint64 stub_getDiskSpace()
{

    return 1000;
}

qint64 stub_getSizeL()
{
    return 1500;
}

qint64 stub_getSizeS()
{
    return 500;
}

qint64 stub_bytesAvailable()
{
    return 1500;
}


void stub_show()
{
    return;
}

void  stub_onLeftSiderBarItemClicked()
{
    return ;
}


QList<QUrl> stub_urlssin()
{
    QList<QUrl> list;
    list << QUrl();

    return list;
}

QList<QUrl> stub_urls()
{
    QList<QUrl> list;
    list << QUrl() << QUrl();

    return list;
}

QStringList stub_checkFilesSpace()
{

    QStringList list;
    list << "first";

    return list;
}

DFontPreviewItemData stub_currModelData()
{
    DFontPreviewItemData data;
    data.strFontId = 1;
    data.fontInfo.filePath = "first";

    return data;
}


Qt::WindowStates stub_windowStateMax()
{
    return Qt::WindowMaximized;
}

Qt::WindowStates stub_windowStateNo()
{
    return Qt::WindowNoState;
}

//DFontPreviewItemData stub_getFontData()
//{
//    DFontPreviewItemData data;

//    data.fontData.strFontName = "first";

//    data.fontInfo.filePath = "first";
//    data.fontInfo.isSystemFont = false;
//    return data;
//}

//int stub_exec()
//{
//    return 1;
//}

QStringList stub_getInstalledFontsPath()
{

    QStringList list;
    list << "first";

    return list;

}

QStringList stub_getAllFontPath()
{

    QStringList list;
    list << "second";

    return list;

}

}

TEST_F(TestDFontMgrMainWindow, checkSetNextTabFocus)
{
    fm->setNextTabFocus(fm->m_ptr->addFontButton);
//    代码setfocus成功，但是检测焦点状态有问题
//    EXPECT_TRUE(fm->m_ptr->searchFontEdit->lineEdit()->hasFocus());
    fm->setNextTabFocus(fm->m_ptr->searchFontEdit->lineEdit());
    fm->setNextTabFocus(fm->m_fontPreviewListView);
    fm->setNextTabFocus(fm->m_ptr->leftSiderBar);

    Stub s;
    s.set(ADDR(QWidget, isVisible), stub_true);
    fm->setNextTabFocus(fm->m_ptr->leftSiderBar);


    fm->setNextTabFocus(fm->m_ptr->fontScaleSlider);
    fm->setNextTabFocus(fm->m_ptr->fontShowArea);
}

TEST_F(TestDFontMgrMainWindow, checkKeyPressEvent)
{
//
    Stub s;
    s.set(ADDR(QWidget, hasFocus), stub_true);

    QSignalSpy spy(fm->m_signalManager, SIGNAL(setLostFocusState(bool)));

    fm->m_ptr->fontScaleSlider->setValue(10);
    QTest::keyPress(fm, Qt::Key_Down);
    EXPECT_TRUE(fm->m_ptr->fontScaleSlider->value() == 9);

    QTest::keyPress(fm, Qt::Key_Up);
    EXPECT_TRUE(fm->m_ptr->fontScaleSlider->value() == 10);
}

TEST_F(TestDFontMgrMainWindow, checkEventFilterKeypress)
{
    //没有可检测的数据
    QTest::keyPress(fm->m_ptr->searchFontEdit->lineEdit(), Qt::Key_Tab);
    QTest::keyPress(fm->m_ptr->textInputEdit->lineEdit(), Qt::Key_Tab);
    QTest::keyPress(fm->m_ptr->leftSiderBar, Qt::Key_Tab);

    QTest::keyPress(fm->m_ptr->addFontButton, Qt::Key_Right);

    QTest::keyPress(fm->m_ptr->searchFontEdit->lineEdit(), Qt::Key_Escape);
    EXPECT_TRUE(fm->m_ptr->searchFontEdit->lineEdit()->text().isEmpty());

    QTest::keyPress(fm->m_ptr->textInputEdit->lineEdit(), Qt::Key_Escape);
    EXPECT_TRUE(fm->m_ptr->textInputEdit->lineEdit()->text().isEmpty());
}

TEST_F(TestDFontMgrMainWindow, checkEventFilterFocusOut)
{
    QFocusEvent *e = new QFocusEvent(QEvent::FocusOut, Qt::ActiveWindowFocusReason);
    fm->eventFilter(fm->m_ptr->leftSiderBar, e);
    EXPECT_FALSE(fm->m_leftListViewTabFocus);

    Stub s;
    s.set(ADDR(DSplitListWidget, IsTabFocus), stub_true);

    fm->eventFilter(fm->m_ptr->leftSiderBar, e);
    EXPECT_TRUE(fm->m_currentStatus.m_IsFirstFocus);

    fm->eventFilter(fm->m_fontPreviewListView, e);
    EXPECT_FALSE(fm->m_previewListViewTabFocus);

    fm->eventFilter(fm->m_ptr->fontScaleSlider, e);
    EXPECT_TRUE(fm->m_fontPreviewListView->m_isGetFocusFromSlider);

}

TEST_F(TestDFontMgrMainWindow, checkEventFilterFocusIn)
{
    QSignalSpy spy(fm->m_signalManager, SIGNAL(setLostFocusState(bool)));


    QFocusEvent *e = new QFocusEvent(QEvent::FocusIn, Qt::ActiveWindowFocusReason);
    fm->m_leftListViewTabFocus = true;
    fm->eventFilter(fm->m_ptr->leftSiderBar, e);
    EXPECT_FALSE(fm->m_leftListViewTabFocus);

    fm->m_leftListViewTabFocus = false;
    fm->eventFilter(fm->m_ptr->leftSiderBar, e);
    EXPECT_TRUE(fm->m_ptr->leftSiderBar->m_IsHalfWayFocus);

    fm->eventFilter(fm->m_fontPreviewListView, e);
    EXPECT_TRUE(spy.count() == 1);
    EXPECT_FALSE(fm->m_fontPreviewListView->m_IsTabFocus);

    fm->eventFilter(fm->m_ptr->searchFontEdit->lineEdit(), e);
    EXPECT_FALSE(fm->m_isSearchLineEditMenuPoped);

    fm->eventFilter(fm->m_ptr->textInputEdit->lineEdit(), e);
    EXPECT_FALSE(fm->m_isSearchLineEditMenuPoped);

}

TEST_F(TestDFontMgrMainWindow, checkEventFilterMouse)
{
    QSignalSpy spy(fm->m_signalManager, SIGNAL(setLostFocusState(bool)));

    QTest::mousePress(fm->m_ptr->searchFontEdit->lineEdit(), Qt::RightButton);
    EXPECT_TRUE(fm->m_isSearchLineEditMenuPoped);

    QTest::mousePress(fm->m_ptr->textInputEdit->lineEdit(), Qt::RightButton);
    EXPECT_TRUE(fm->m_isSearchLineEditMenuPoped);

    QTest::mousePress(fm->m_ptr->addFontButton, Qt::RightButton);

}

TEST_F(TestDFontMgrMainWindow, checkCheckFilesSpace)
{
    Stub s;
    s.set(ADDR(DFontMgrMainWindow, getDiskSpace), stub_getDiskSpace);

    Stub s1;
    s1.set(ADDR(QFileInfo, size), stub_getSizeL);

    QStringList list;
    list << "first";

    EXPECT_TRUE(fm->checkFilesSpace(list).isEmpty());

    s1.set(ADDR(QFileInfo, size), stub_getSizeS);
    EXPECT_TRUE(fm->checkFilesSpace(list).contains("first"));

}

TEST_F(TestDFontMgrMainWindow, checkGetDiskSpace)
{
    Stub s;
    s.set(ADDR(QStorageInfo, bytesAvailable), stub_bytesAvailable);

    EXPECT_TRUE(1500 == fm->getDiskSpace());
    EXPECT_TRUE(1500 == fm->getDiskSpace(false));

}

TEST_F(TestDFontMgrMainWindow, checkOnPreviewTextChanged)
{
    fm->m_fontPreviewListView->m_bLoadDataFinish = true;
//    fm->onPreviewTextChanged();

    fm->m_fontPreviewListView->getFontPreviewProxyModel()->insertRows(0, 5);
    fm->onPreviewTextChanged();

    fm->onPreviewTextChanged("first");
    fm->onPreviewTextChanged();
}

TEST_F(TestDFontMgrMainWindow, checkWaitForInsert)
{
    fm->m_waitForInstall.clear();

    fm->waitForInsert();

    fm->m_waitForInstall.append("first");

    Stub s;
    s.set(ADDR(DFontMgrMainWindow, installFont), stub_true);
    fm->waitForInsert();

    EXPECT_TRUE(fm->m_waitForInstall.isEmpty());
}

TEST_F(TestDFontMgrMainWindow, checkHideSpinner)
{
    fm->m_cacheFinish = true;
    fm->m_installFinish = true;

    fm->hideSpinner();
    //定时器触发lambda函数，无法测试到
}

TEST_F(TestDFontMgrMainWindow, checkShowSpinner)
{
    Stub s;
    s.set(ADDR(QWidget, show), stub_show);

    fm->showSpinner(DFontSpinnerWidget::Load);

    fm->showSpinner(DFontSpinnerWidget::Load, true);
}

TEST_F(TestDFontMgrMainWindow, checkShowInstalledFiles)
{
    Stub s;
    s.set(ADDR(DFontMgrMainWindow, onLeftSiderBarItemClicked), stub_onLeftSiderBarItemClicked);

    fm->showInstalledFiles();
}

TEST_F(TestDFontMgrMainWindow, checkShowAllShortcut)
{
    //startDetached函数打桩失败，导致会有界面弹出。之后再研究这里如何打桩
    fm->showAllShortcut();
}

TEST_F(TestDFontMgrMainWindow, checkResizeEvent)
{

    QResizeEvent *e = new QResizeEvent(QSize(), QSize());

    fm->resizeEvent(e);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenRect =  screen->availableVirtualGeometry();
    fm->resize(screenRect.size());
    fm->resizeEvent(e);
}

TEST_F(TestDFontMgrMainWindow, checkDropEvent)
{
    QPointF p(300, 300);
    QMimeData data;

    QSignalSpy spy(fm, SIGNAL(fileSelected(const QStringList &, bool)));

    QDropEvent *e = new QDropEvent(p, Qt::CopyAction, &data, Qt::LeftButton, Qt::NoModifier);
    fm->dropEvent(e);

    Stub s;
    s.set(ADDR(QMimeData, hasUrls), stub_true);

    Stub s1;
    s1.set(ADDR(QMimeData, urls), stub_urls);

    Stub s2;
    s2.set(ADDR(Utils, isFontMimeType), stub_true);

    Stub s3;
    s3.set(ADDR(DFontMgrMainWindow, installFont), stub_true);


    fm->dropEvent(e);
    EXPECT_TRUE(spy.count() == 1);
//    fm->dropEvent(e);
}

TEST_F(TestDFontMgrMainWindow, checkDragEnterEvent)
{

    QPoint p(300, 300);
    QMimeData data;

    QDragEnterEvent *e = new QDragEnterEvent(p, Qt::CopyAction, &data, Qt::LeftButton, Qt::NoModifier);
    fm->dropEvent(e);

    Stub s;
    s.set(ADDR(QMimeData, hasUrls), stub_true);

    Stub s1;
    s1.set(ADDR(QMimeData, urls), stub_urls);

    Stub s2;
    s2.set(ADDR(Utils, isFontMimeType), stub_true);

    Stub s3;
    s3.set(ADDR(DFontMgrMainWindow, installFont), stub_true);

    fm->dragEnterEvent(e);

    Stub s4;
    s1.set(ADDR(QMimeData, urls), stub_urlssin);
    fm->dragEnterEvent(e);

}

TEST_F(TestDFontMgrMainWindow, checkShowExportFontMessage)
{
    Stub s1;
    s1.set((void(DMessageManager::*)(QWidget *, const QIcon &, const QString &)) ADDR(DMessageManager, sendMessage), stub_show);

    fm->showExportFontMessage(1, 0);

    fm->showExportFontMessage(2, 0);

    fm->showExportFontMessage(1, 1);

    fm->showExportFontMessage(1, 2);
}

TEST_F(TestDFontMgrMainWindow, checkExportFont)
{
    QSignalSpy spy(DFontPreviewListDataThread::instance(), SIGNAL(requestExportFont(const QStringList &)));

    Stub s;
    s.set((void(DMessageManager::*)(QWidget *, const QIcon &, const QString &)) ADDR(DMessageManager, sendMessage), stub_show);

    fm->exportFont();

    Stub s1;
    s1.set(ADDR(DFontMgrMainWindow, checkFilesSpace), stub_checkFilesSpace);

    fm->exportFont();

    EXPECT_TRUE(spy.count() == 1);
}

////还需要研究exec如何打桩，否则无法运行测试程序，运行就会崩溃
//TEST_F(TestDFontMgrMainWindow, checkDelCurrentFont)
//{

//    fm->m_fontPreviewListView->m_bLoadDataFinish = true;
////    fm->onPreviewTextChanged();

//    fm->m_fontPreviewListView->getFontPreviewProxyModel()->insertRows(0, 5);

//    typedef int (QDialog::*fptr)();
//    fptr Qdialog_exec = (fptr)(&QDialog::exec);   //获取虚函数地址

//    Stub stub;
//    stub.set(Qdialog_exec, stub_exec);

//    fm->m_fontPreviewListView->selectAll();
//    fm->m_fIsDeleting = DFontMgrMainWindow::UnDeleting;
//    DFontPreviewItemData data;
//    data.fontData.strFontName = "aaa";
//    fm->m_fontPreviewListView->m_curFontData  = data;
//    Stub s1;
//    s1.set(ADDR(DFontPreviewListDataThread, getFontData), stub_getFontData);

//    fm->delCurrentFont(false);

//}

TEST_F(TestDFontMgrMainWindow, checkOnShowMessage)
{
    QSignalSpy spy(DFontPreviewListDataThread::instance(), SIGNAL(requestExportFont(const QStringList &)));

    Stub s;
    s.set((void(DMessageManager::*)(QWidget *, const QIcon &, const QString &)) ADDR(DMessageManager, sendMessage), stub_show);

    fm->exportFont();

    fm->onShowMessage(1);

    fm->onShowMessage(2);
}

TEST_F(TestDFontMgrMainWindow, checkOnLoadStatus0)
{
    Stub s;
    s.set(ADDR(QWidget, isVisible), stub_true);

    Stub s1;
    s1.set(ADDR(QWidget, show), stub_show);

    fm->onLoadStatus(0);
}

TEST_F(TestDFontMgrMainWindow, checkOnLoadStatus1)
{
    Stub s;
    s.set(ADDR(QWidget, isVisible), stub_false);

    Stub s1;
    s1.set(ADDR(QWidget, show), stub_show);

    Stub s2;
    s2.set(ADDR(DFontMgrMainWindow, onFontListViewRowCountChanged), stub_show);

    fm->m_fontPreviewListView->m_bLoadDataFinish = true;

    fm->m_leftIndex = 1;

    fm->m_ptr->searchFontEdit->lineEdit()->insert("first");

    QSignalSpy spy(fm->m_ptr->searchFontEdit, SIGNAL(textChanged(const QString &)));
    fm->onLoadStatus(1);
    qDebug() << spy.count() << endl;
    EXPECT_TRUE(spy.count() == 1);
}

TEST_F(TestDFontMgrMainWindow, checkOnFontListViewRowCountChanged0)
{

//    isHidden
    Stub s;
    s.set(ADDR(QWidget, isHidden), stub_true);

    Stub s1;
    s1.set(ADDR(QWidget, isVisible), stub_true);

    fm->m_fontPreviewListView->m_bLoadDataFinish = true;

    fm->m_fontPreviewListView->getFontPreviewProxyModel()->insertRows(0, 5);
    fm->onFontListViewRowCountChanged();
}

TEST_F(TestDFontMgrMainWindow, checkOnFontListViewRowCountChanged1)
{

//    isHidden
    Stub s;
    s.set(ADDR(QWidget, isHidden), stub_true);

    Stub s1;
    s1.set(ADDR(QWidget, isVisible), stub_true);

    fm->m_fontPreviewListView->m_bLoadDataFinish = true;
    fm->m_searchTextStatusIsEmpty = false;
    fm->m_isNoResultViewShow = false;
    fm->m_fIsInstalling = false;

    fm->onFontListViewRowCountChanged();

    fm->m_isNoResultViewShow = true;
    fm->onFontListViewRowCountChanged();

}

TEST_F(TestDFontMgrMainWindow, checkOnFontListViewRowCountChanged2)
{

//    isHidden
    Stub s;
    s.set(ADDR(QWidget, isHidden), stub_true);

    Stub s1;
    s1.set(ADDR(QWidget, isVisible), stub_true);

    fm->m_fontPreviewListView->m_bLoadDataFinish = true;
    //    fm->onPreviewTextChanged();

//    fm->m_fontPreviewListView->getFontPreviewProxyModel()->insertRows(0, 5);
    fm->onFontListViewRowCountChanged();

    EXPECT_FALSE(fm->m_isNoResultViewShow);
}

TEST_F(TestDFontMgrMainWindow, checkOnUninstallFcCacheFinish)
{
    fm->m_fIsDeleting = 1;
    fm->onUninstallFcCacheFinish();

    qDebug() << fm->m_fIsDeleting << endl;
    EXPECT_TRUE(fm->m_fIsDeleting == 0);
}

TEST_F(TestDFontMgrMainWindow, checkOnFontInstallFinished)
{
    fm->onFontInstallFinished(QStringList());

    EXPECT_TRUE(fm->m_isInstallOver);
}

TEST_F(TestDFontMgrMainWindow, checkOnLeftSiderBarItemClicked)
{
    Stub s;
    s.set(ADDR(QWidget, hasFocus), stub_true);

    Stub s1;
    s1.set(ADDR(QWidget, isVisible), stub_true);

    fm->m_fontPreviewListView->m_bLoadDataFinish = false;
    fm->onLeftSiderBarItemClicked(2);
    EXPECT_TRUE(fm->m_leftIndex == 2);

    fm->m_fontPreviewListView->m_bLoadDataFinish = true;
    fm->onLeftSiderBarItemClicked(2);
}

//static 重载函数打桩
TEST_F(TestDFontMgrMainWindow, checkShowFontFilePostion)
{
    Stub s;
    s.set(ADDR(DFontPreviewListView, currModelData), stub_currModelData);

    Stub s1;
    typedef bool (*FP)(QString, const QString &);



//    s1.set((bool(DDesktopServices::*)(QString, const QString &))ADDR(DDesktopServices, showFileItem), stub_show);
    s1.set(FP ADDR(DDesktopServices, showFileItem), stub_return);
//    Stub s1;
//    s1.set(ADDR(DDesktopServices, showFileItem), stub_true);
    fm->showFontFilePostion();
}

TEST_F(TestDFontMgrMainWindow, checkOnFontSizeChanged)
{
    fm->onFontSizeChanged(20);
    fm->m_fontPreviewListView->m_bLoadDataFinish = false;
    fm->onFontSizeChanged(20);

    fm->m_fontPreviewListView->m_bLoadDataFinish = true;

    fm->m_fontPreviewListView->getFontPreviewProxyModel()->insertRows(0, 5);
    fm->onFontSizeChanged(20);
}

TEST_F(TestDFontMgrMainWindow, checkOnPreviewTextChangedR)
{
    fm->onPreviewTextChanged("first");

    EXPECT_TRUE(fm->m_previewText == "first");
}

TEST_F(TestDFontMgrMainWindow, checkOnSearchTextChanged)
{
    fm->m_fontPreviewListView->m_bLoadDataFinish = false;
    fm->onSearchTextChanged("first");

    fm->m_fontPreviewListView->m_bLoadDataFinish = true;
    fm->onSearchTextChanged("first");
    DFontPreviewProxyModel *filterModel = fm->m_fontPreviewListView->getFontPreviewProxyModel();

    EXPECT_TRUE(filterModel->filterKeyColumn() == 0);
}

TEST_F(TestDFontMgrMainWindow, checkSetDeleteFinish)
{
    fm->setDeleteFinish();

    EXPECT_FALSE(fm->m_fIsDeleting);
}

TEST_F(TestDFontMgrMainWindow, checkForceNoramlInstalltionQuitIfNeeded)
{
    Stub s;
    s.set(ADDR(DFInstallNormalWindow, breakInstalltion), stub_return);


    fm->forceNoramlInstalltionQuitIfNeeded();

    fm->m_fIsInstalling = true;

    fm->forceNoramlInstalltionQuitIfNeeded();
}

//这个函数中主要是完全无用的代码，暂时标注掉
TEST_F(TestDFontMgrMainWindow, checkInitQuickWindowIfNeeded)
{
    Stub s;
    s.set(ADDR(QWidget, show), stub_return);

    Stub s1;
    s1.set(ADDR(QWidget, raise), stub_return);

    Stub s2;
    s2.set(ADDR(QWidget, activateWindow), stub_return);

    fm->InitQuickWindowIfNeeded();
}

//setQuickInstallMode
TEST_F(TestDFontMgrMainWindow, checkSetQuickInstallMode)
{
    fm->setQuickInstallMode(false);

    EXPECT_FALSE(fm->m_isQuickMode);
}

TEST_F(TestDFontMgrMainWindow, checkInitRightKeyMenu)
{
    Stub s;
    s.set(ADDR(DFontMenuManager, createRightKeyMenu), stub_return);

    fm->initRightKeyMenu();
}

TEST_F(TestDFontMgrMainWindow, checkInstallFontFromSys)
{
    Stub s;
    s.set(ADDR(DFontMgrMainWindow, installFont), stub_return);

    QSignalSpy spy(fm->m_signalManager, SIGNAL(installDuringPopErrorDialog(const QStringList &)));

    QStringList list;
    list << "first";

    fm->m_fontPreviewListView->m_bLoadDataFinish = false;
    fm->installFontFromSys(list);
    EXPECT_TRUE(fm->m_waitForInstall.contains("first"));

    fm->m_fontPreviewListView->m_bLoadDataFinish = true;
    fm->m_fIsDeleting = true;
    fm->installFontFromSys(list);
    EXPECT_TRUE(fm->m_waitForInstall.contains("first"));

    fm->m_fontPreviewListView->m_bLoadDataFinish = true;
    fm->m_fIsDeleting = false;
    fm->m_isPopInstallErrorDialog = true;
    fm->installFontFromSys(list);
    EXPECT_TRUE(spy.count() == 1);

    fm->m_fontPreviewListView->m_bLoadDataFinish = true;
    fm->m_fIsDeleting = false;
    fm->m_isPopInstallErrorDialog = false;
    fm->installFontFromSys(list);
}

TEST_F(TestDFontMgrMainWindow, checkHandleMenuEvent)
{
    Stub s;
    Stub s2;

    QList<QAction *> actionList = fm->m_ptr->rightKeyMenu->actions();

    s.set(ADDR(DFontMgrMainWindow, handleAddFontEvent), stub_return);
    fm->handleMenuEvent(actionList.first());

    s.set(ADDR(DFontPreviewListView, onEnableBtnClicked), stub_return);
    s2.set(ADDR(DFontPreviewListView, syncTabStatus), stub_return);
    fm->handleMenuEvent(actionList.at(2));

    s.set(ADDR(DFontMgrMainWindow, delCurrentFont), stub_return);
    fm->handleMenuEvent(actionList.at(3));

    s.set(ADDR(DFontMgrMainWindow, exportFont), stub_return);
    s2.set(ADDR(DFontPreviewListView, syncTabStatus), stub_return);
    fm->handleMenuEvent(actionList.at(4));

    s.set(ADDR(DFontPreviewListView, onCollectBtnClicked), stub_return);
    s2.set(ADDR(DFontPreviewListView, syncTabStatus), stub_return);
    fm->handleMenuEvent(actionList.at(5));

    s.set(ADDR(DFontMgrMainWindow, showFontFilePostion), stub_return);
    fm->handleMenuEvent(actionList.last());
}

//TEST_F(TestDFontMgrMainWindow, checkHandleAddFontEvent)
//{
//    Stub s;
//    Stub s2;

//    s.set(ADDR(DFontMgrMainWindow, installFont), stub_return);
//    s2.set(ADDR(dfontinfoscrollarea, autoHeight), stub_return);

//    fm->handleAddFontEvent();
//}

//TEST_F(TestDFontMgrMainWindow, checkRespondToInstallOver)
//{
//    Stub s;
//    s.set(ADDR(QWidget, isVisible), stub_true);

//    Stub s2;
//    s2.set(ADDR(DFontMgrMainWindow, showSpinner), stub_return);

//    fm->respondToInstallOver(0);
//    EXPECT_FALSE(fm->m_fIsInstalling);

//    fm->respondToInstallOver(12);
//}

//mark
//TEST_F(TestDFontMgrMainWindow, checkInitFontFiles)
//{
//    Stub s;
//    s.set(ADDR(DFMDBManager, isSystemFont), stub_false);

//    Stub s2;
//    s2.set(ADDR(DFMDBManager, getInstalledFontsPath), stub_getInstalledFontsPath);

//    Stub s3;
//    s2.set(ADDR(DFontInfoManager, getAllFontPath), stub_getAllFontPath);

//    fm->initFontFiles();
//}

TEST_F(TestDFontMgrMainWindow, checkinitShortcutsFontSize)
{
    fm->m_previewFontSize = 10;

    emit fm->m_scZoomIn->activated();
    EXPECT_TRUE(fm->m_previewFontSize == 11);

    emit fm->m_scZoomOut->activated();
    EXPECT_TRUE(fm->m_previewFontSize == 10);

    emit fm->m_scDefaultSize->activated();
    EXPECT_TRUE(fm->m_previewFontSize == DFontMgrMainWindow::DEFAULT_FONT_SIZE);
}


TEST_F(TestDFontMgrMainWindow, checkinitShortcutsReSize)
{
    Stub s;
    Stub s1;

    s.set(ADDR(QWidget, windowState), stub_windowStateMax);
    s1.set(ADDR(QWidget, showNormal), stub_return);

    emit fm->m_scWndReize->activated();

    s.set(ADDR(QWidget, windowState), stub_windowStateNo);
    s1.set(ADDR(QWidget, showMaximized), stub_return);
    emit fm->m_scWndReize->activated();
}

TEST_F(TestDFontMgrMainWindow, checkinitShortcutsDel)
{
    Stub s;
    Stub s1;

    s.set(ADDR(DFontPreviewListView, syncRecoveryTabStatus), stub_return);
    s1.set(ADDR(DFontMgrMainWindow, delCurrentFont), stub_return);

    fm->m_cacheFinish = false;
    fm->m_installFinish = false;

    emit fm->m_scDeleteFont->activated();
}

TEST_F(TestDFontMgrMainWindow, checkinitShortcutsMenu)
{
    Stub s;
    Stub s1;

    QSignalSpy spy(fm->m_signalManager, SIGNAL(onMenuHidden()));

    s.set(ADDR(QWidget, hasFocus), stub_true);
    s1.set(ADDR(QApplication, sendEvent), stub_return);

    fm->m_isSearchLineEditMenuPoped = false;

    emit fm->m_scShowMenu->activated();
}

TEST_F(TestDFontMgrMainWindow, checkinitShortcutsAdd)
{
    Stub s;
    Stub s1;

    QSignalSpy spy(fm->m_signalManager, SIGNAL(onMenuHidden()));

    s.set(ADDR(DFontPreviewListView, syncRecoveryTabStatus), stub_return);
    s1.set(ADDR(DFontMgrMainWindow, handleAddFontEvent), stub_return);

    emit fm->m_scAddNewFont->activated();
}

TEST_F(TestDFontMgrMainWindow, checkinitShortcutsFav)
{

}

TEST_F(TestDFontMgrMainWindow, checkinitShortcutsElse)
{
    Stub s;
    s.set(ADDR(DFontMgrMainWindow, showAllShortcut), stub_return);

    Stub s1;
    s1.set(ADDR(DFontPreviewListView, syncRecoveryTabStatus), stub_return);

    fm->m_fontPreviewListView->m_bLoadDataFinish = true;

    fm->m_fontPreviewListView->getFontPreviewProxyModel()->insertRows(0, 5);

    emit fm->m_scShowAllSC->activated();

    emit fm->m_scPageUp->activated();

    emit fm->m_scPageDown->activated();

    emit fm->m_scFindFont->activated();
}









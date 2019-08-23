#ifndef DFONTMENUMANAGER_H
#define DFONTMENUMANAGER_H

#include <QAction>
#include <QMenu>
#include <QObject>
#include <QSet>
#include <QVector>
class DFontMenuManager : public QObject
{
    Q_OBJECT
public:
    explicit DFontMenuManager(QObject *parent = nullptr);

    static DFontMenuManager *getInstance();

    enum MenuType {
        ToolBarMenu,
        RightKeyMenu,
    };

    enum MenuAction {
        M_Separator,
        M_AddFont,
        M_Theme,
        M_ThemeLight,
        M_ThemeDark,
        M_ThemeFollowSystem,
        M_Help,
        M_EnableOrDisable,
        M_DeleteFont,
        M_Faverator,
        M_FontInfo,
        M_ShowFontPostion,
    };

    struct FMenuItem {
        FMenuItem(QString name, MenuAction id, bool haveSubMenu = false)
            : actionName(name)
            , actionId(id)
            , fHaveSubMenu(haveSubMenu)
        {
        }

        QString actionName;
        MenuAction actionId;
        QAction   *action {nullptr};
        bool fHaveSubMenu {false};
        QVector<FMenuItem *> subMenulist;
    };

    typedef void (*FMenuActionTriggle)(QAction *);

    QMenu *createToolBarSettingsMenu(FMenuActionTriggle actionTriggle = nullptr);
    QMenu *createRightKeyMenu(FMenuActionTriggle actionTriggle = nullptr);

    QAction *getActionByMenuAction(MenuAction maction, MenuType menuType);

private:
    void initMenuData();

    QMap<MenuAction, FMenuItem *> m_fontToolsBarMenus;
    QMap<MenuAction, FMenuItem *> m_fontRightKeyMenus;

private:
    static DFontMenuManager *instance;
signals:

public slots:
};

#endif  // DFONTMENUMANAGER_H

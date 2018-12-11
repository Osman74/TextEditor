#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
#include <QWidget>
#include <QObject>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QLayout>
#include <QTabWidget>
#include <QStatusBar>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <string>
#include <QToolBar>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QFileSystemModel>
#include <QStringListModel>
#include <QStringList>
#include <QTreeView>
#include <QListView>
#include <QComboBox>
#include <QAction>
#include <QStyleFactory>
#include <QHeaderView>
#include <vector>
#include "CPPhighlighter.h"
#include "highlighter.h"
#include "CPPhighlighter.h"
#include "formsyntax.h"

class TextEditor : public QMainWindow{
    Q_OBJECT
private:
    QTabWidget* m_tbw;
    int m_CountDefaultFiles;
    QFileSystemModel *m_fileModel;
    struct Tab
        {
            QTextEdit* m_textEdit;
            QFile* m_file;
            bool m_saved = true;
        };
    QComboBox* m_ComboBoxPath;
    QStringListModel* m_StringListModel;
    QStringList m_StringList;
    std::vector<bool> m_VectorFlagsChanged;

    QDockWidget* Explorer;
    QDockWidget* ExplorerOpenDocs;

    FormSyntax* m_FormSyntax;
public:
    TextEditor(QWidget* pwgt = 0);
protected:
    virtual void closeEvent(QCloseEvent*);
    virtual void dropEvent(QDropEvent* pe);
    virtual void dragEnterEvent(QDragEnterEvent* pe)
    {
        if (pe->mimeData()->hasFormat("text/uri-list")) {
        pe->acceptProposedAction();
        }
    }
private slots:
    void slotNewDoc();
    void slotOpen();
    void slotChanged();
    void slotSave();
    void slotSaveAs( );
    void slotSavePath();
    void slotSaveAll();
    void slotClose();
    void slotCloseAll();
    void slotExit();
    void slotCut();
    void slotCopy();
    void slotPaste();
    void slotDelete();
    void slotSelectAll();
    void slotViewExplorer(bool);
    void slotViewExplorerOpenDocuments(bool);
    void slotExplorerSetDoc(QModelIndex);
    void slotSetCurrentDoc(int);
    void slotCloseTab(int);
    void slotSelectCPP();
    void slotSelectSyntax();
    void slotAddSyntax();
};
#endif // TEXTEDITOR_H

#include "texteditor.h"

TextEditor::TextEditor(QWidget* pwgt){
    setAcceptDrops(true);
    setStyle(QStyleFactory::create("Fusion"));
    setWindowTitle("TextEditor");
    setWindowIcon(QIcon(":new/prefix1/resourses/text-editor.png"));
    QToolBar* ptb = new QToolBar("Панель инструментов");
    ptb->setStyleSheet("background-color:darkgray;");
    QMenu* pmnuFile = new QMenu("Файл");
    QAction * pactNew = new QAction ("Новый файл", 0) ;
    pactNew -> setText ("&Новый");
    pactNew -> setShortcut ( QKeySequence ("CTRL+N"));
    pactNew -> setToolTip ("Создать новый документ");
    pactNew -> setStatusTip ("Создание нового файла");
    pactNew -> setWhatsThis ("Создание нового файла");
    pactNew -> setIcon (QPixmap(":new/prefix1/resourses/New.png"));
    connect (pactNew, SIGNAL(triggered()), SLOT(slotNewDoc()));
    ptb->addAction(pactNew);
    pmnuFile->addAction(pactNew);

    QAction * pactOpen = new QAction ("Открыть файл", 0) ;
    pactOpen -> setText ("&Открыть");
    pactOpen -> setShortcut ( QKeySequence ("CTRL+O"));
    pactOpen -> setToolTip ("Открыть файл");
    pactOpen -> setStatusTip ("Открытие файла");
    pactOpen -> setWhatsThis ("Открытие файла");
    pactOpen -> setIcon (QPixmap(":new/prefix1/resourses/Open.png"));
    connect (pactOpen, SIGNAL(triggered()), SLOT(slotOpen()));
    ptb->addAction(pactOpen);
    pmnuFile->addAction(pactOpen);
    //ptb->addSeparator();

    QAction * pactSave = new QAction ("Сохранить файл", 0) ;
    pactSave -> setText ("&Сохранить");
    pactSave -> setShortcut ( QKeySequence ("CTRL+S"));
    pactSave -> setToolTip ("Сохранить файл");
    pactSave -> setStatusTip ("Сохранение файла");
    pactSave -> setWhatsThis ("Сохранение файла");
    pactSave -> setIcon (QPixmap(":new/prefix1/resourses/Save.png"));
    connect (pactSave, SIGNAL(triggered()), SLOT(slotSave()));
    ptb->addAction(pactSave);
    pmnuFile->addAction(pactSave);

    pmnuFile->addAction("Сохранить как...", this, SLOT(slotSaveAs()));

    QAction* pactSaveAll = new QAction ("Сохранить все файлы", 0) ;
    pactSaveAll -> setText ("&Сохранить все");
    pactSaveAll -> setToolTip ("Сохранить все файлы");
    pactSaveAll -> setStatusTip ("Сохранение всех файлов");
    pactSaveAll -> setWhatsThis ("Сохранение всех файлов");
    pactSaveAll -> setIcon (QPixmap(":new/prefix1/resourses/SaveAll.png"));
    connect (pactSaveAll, SIGNAL(triggered()), SLOT(slotSaveAll()));
    ptb->addAction(pactSaveAll);
    pmnuFile->addAction(pactSaveAll);
    pmnuFile->addAction("Закрыть", this, SLOT(slotClose()));
    pmnuFile->addAction("Закрыть все", this, SLOT(slotCloseAll()));
    pmnuFile->addAction("Выход", this, SLOT(slotExit()));
    QMenu* pmnuEdit = new QMenu("Правка");
    pmnuEdit->addAction("Вырезать", this, SLOT(slotCut()));
    pmnuEdit->addAction("Копировать");
    pmnuEdit->addAction("Вставить");
    pmnuEdit->addAction("Удалить");
    pmnuEdit->addAction("Выделить все");
    QMenu* pmnuView = new QMenu("Вид");
    QAction* pactViewExplorer = new QAction ("Показать проводник", 0);
    pactViewExplorer->setCheckable(true);
    pactViewExplorer->setChecked(true);
    connect (pactViewExplorer, SIGNAL(toggled(bool)), SLOT(slotViewExplorer(bool)));

    pmnuView->addAction(pactViewExplorer);
    QAction* pactViewExplorerOpenDocuments = new QAction ("Показать обозреватель открытых документов", 0);
    pactViewExplorerOpenDocuments->setCheckable(true);
    pactViewExplorerOpenDocuments->setChecked(true);
    connect (pactViewExplorerOpenDocuments, SIGNAL(toggled(bool)), SLOT(slotViewExplorerOpenDocuments(bool)));
    pmnuView->addAction(pactViewExplorerOpenDocuments);

    this->menuBar()->addMenu(pmnuFile);
    this->menuBar()->addMenu(pmnuEdit);
    this->menuBar()->addMenu(pmnuView);

    QMenu* pmnuSyntax = new QMenu("Синтаксисы");
    pmnuSyntax->addAction("C++", this, SLOT(slotSelectCPP()));
    pmnuSyntax->addAction("Добавить свой синтаксис", this, SLOT(slotAddSyntax()));
    this->menuBar()->addMenu(pmnuSyntax);

    m_tbw = new QTabWidget;
    setCentralWidget(m_tbw);
    statusBar()->showMessage("Готово", 3000);

    addToolBar(Qt::TopToolBarArea, ptb);

    m_fileModel = new QFileSystemModel;
    QTreeView* treeView = new QTreeView();
    treeView->setModel(m_fileModel);
    m_fileModel->setRootPath(QDir::currentPath());
    treeView->setWindowTitle("Проводник");
    treeView->setDragDropMode(QAbstractItemView::InternalMove);
    QHeaderView* pTreeHeader = treeView->header();
    pTreeHeader->setStretchLastSection(false);
    pTreeHeader->setSectionResizeMode(0, QHeaderView::Stretch);
    //pTreeHeader->setResizeMode(nRowSize, QHeaderView::Interactive);
    Explorer = new QDockWidget;
    Explorer->setWidget(treeView);
    Explorer->resize(500, 1000);
    Explorer->setWindowTitle("Проводник");
    addDockWidget(Qt::LeftDockWidgetArea, Explorer);

    m_StringListModel = new QStringListModel;
    m_StringListModel->setStringList(m_StringList);
    QListView* listView = new QListView();
    listView->setWindowTitle("Обозреватель открытых документов");
    listView->setModel(m_StringListModel);
    ExplorerOpenDocs = new QDockWidget;
    //QDockWidget* DW = new QDockWidget("ExplorerOpenDocuments", this);
    ExplorerOpenDocs->setWidget(listView);
    ExplorerOpenDocs->resize(500, 500);
    ExplorerOpenDocs->setWindowTitle("Обозреватель открытых документов");
    addDockWidget(Qt::RightDockWidgetArea, ExplorerOpenDocs);

    m_tbw->setTabsClosable(true);

    m_CountDefaultFiles = 0;

    m_FormSyntax = new FormSyntax();
    m_FormSyntax->setWindowTitle("Свой синтаксис");
    m_FormSyntax->setWindowIcon(QIcon(":new/prefix1/resourses/text-editor.png"));
    m_FormSyntax->resize(700, 500);
    connect(m_FormSyntax, SIGNAL(signalClose()), this, SLOT(slotSelectSyntax()));

    connect(m_tbw, SIGNAL(tabCloseRequested(int)), this, SLOT(slotCloseTab(int)));
    connect(listView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotExplorerSetDoc(QModelIndex)));
    connect(m_tbw, SIGNAL(tabBarClicked(int)), this, SLOT(slotSetCurrentDoc(int)));
    connect(ExplorerOpenDocs, SIGNAL(visibilityChanged(bool)), pactViewExplorerOpenDocuments, SLOT(setChecked(bool)));
    connect(Explorer, SIGNAL(visibilityChanged(bool)), pactViewExplorer, SLOT(setChecked(bool)));
}


void TextEditor::slotNewDoc()
{
    QTextEdit* TE = new QTextEdit;
    ++m_CountDefaultFiles;
    if(m_CountDefaultFiles > 1)
    {
        char charCountFiles[2];
        itoa(m_CountDefaultFiles - 1, charCountFiles, 10);
        QString countFiles(charCountFiles);
        m_tbw->addTab(TE, QString("Новый документ(") + countFiles + QString(").txt"));
        m_StringList.push_back(QString("Новый документ(") + countFiles + QString(").txt"));
        setWindowTitle(QString("Новый документ(") + countFiles + QString(").txt"));
    }
    else
    {
      m_tbw->addTab(TE, "Новый документ.txt");
      m_StringList.push_back(QString("Новый документ.txt"));
    }
    m_StringListModel->setStringList(m_StringList);
    m_VectorFlagsChanged.push_back(false);
    m_tbw->setCurrentIndex(m_tbw->count() - 1);
    connect(TE, SIGNAL(textChanged()), this, SLOT(slotChanged()));
}

void TextEditor::slotOpen()
{
     QTextEdit* TE = new QTextEdit;
     QString h = QFileDialog::getOpenFileName(this,
                                              "Открыть файл",
                                              QDir::current().dirName(),
                                              "Все файлы(*.*);;Текстовые файлы(*.txt)");
     if(h.isEmpty()) return;
     QFile file(h);
     QString str;
     QString Text;
     QTextStream TS(&file);
     if (file.open(QIODevice::ReadOnly))
         while(!TS.atEnd())
         {
            TS.readLineInto(&str);
            str.append("\n");
            Text.append(str);
         }
     TE->setText(Text);
     QFileInfo fInfo(file);
     m_tbw->addTab(TE, fInfo.baseName() + "." + fInfo.completeSuffix());
     setWindowTitle(fInfo.baseName() + "." + fInfo.completeSuffix());
     m_StringList.push_back(fInfo.baseName());
     m_StringListModel->setStringList(m_StringList);
     if((fInfo.completeSuffix() == "h") || ((fInfo.completeSuffix() == "cpp")))
     {
         new CPPHighlighter("CPPSyntax.xml", TE->document());
     }
     file.close();
     m_tbw->setCurrentIndex(m_tbw->count() - 1);
     m_VectorFlagsChanged.push_back(false);
     connect(TE, SIGNAL(textChanged()), this, SLOT(slotChanged()));
}

void TextEditor::slotChanged()
{
    m_VectorFlagsChanged[m_tbw->currentIndex()] = true;
}

void TextEditor::slotSave()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    QFile file(m_tbw->tabText(0));
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream(&file) << TE->toPlainText();
        file.close();
    }
    m_VectorFlagsChanged[m_tbw->currentIndex()] = false;
}

void TextEditor::slotSaveAs()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    QString str = QFileDialog::getSaveFileName(0, m_tbw->tabText(0), QString(), tr("TXT file(*.txt);;CPP file(*.cpp);;H file (*.h)"), 0);
    QFile file(str);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream(&file) << TE->toPlainText();
        file.close();
    }
    m_VectorFlagsChanged[m_tbw->currentIndex()] = false;
}

void TextEditor::slotSavePath()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    QFile file(m_tbw->tabText(0) + "." + m_ComboBoxPath->currentText());
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream(&file) << TE->toPlainText();
        file.close();
    }
}

void TextEditor::slotSaveAll()
{
    for(int i = 0; i < m_tbw->count(); ++i)
    {
        QTextEdit* TE = (QTextEdit*)m_tbw->widget(i);
        QFile file(m_tbw->tabText(i));
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream(&file) << TE->toPlainText();
            file.close();
        }
    }
//    for(auto &flag : m_VectorFlagsChanged)
//        flag = false;
    for(int i = 0; i < m_VectorFlagsChanged.size(); ++i)
        m_VectorFlagsChanged[i] = false;
}

void TextEditor::slotClose()
{
    int p_Ind = m_tbw->currentIndex();
    if(m_VectorFlagsChanged[m_tbw->currentIndex()] == true)
    {
        QMessageBox* pmbx =
         new QMessageBox(QMessageBox::Information,
         "MessageBox",
         "Сохранить изменения файла?",
         QMessageBox::Yes | QMessageBox::No |
         QMessageBox::Cancel);
        int n = pmbx->exec();
        if (n == QMessageBox::Cancel)
            return;
        if (n == QMessageBox::Yes)
        {
            slotSave();
            m_tbw->removeTab(p_Ind);
        }
        if (n == QMessageBox::No)
        {
            m_tbw->removeTab(p_Ind);
        }
    }
    else m_tbw->removeTab(p_Ind);
    m_StringList.removeAt(p_Ind);
    m_StringListModel->setStringList(m_StringList);
    m_VectorFlagsChanged.erase(m_VectorFlagsChanged.begin() + p_Ind);
}

void TextEditor::slotCloseAll()
{
    qDebug() << m_tbw->count();
    int N = m_tbw->count();
    for(int i = 0; i < N; ++i)
    {
        m_tbw->setCurrentIndex(0);
        if(m_VectorFlagsChanged[0] == true)
        {
            QMessageBox* pmbx =
             new QMessageBox(QMessageBox::Information,
             "MessageBox",
             "Сохранить изменения файла?",
             QMessageBox::Yes | QMessageBox::No |
             QMessageBox::Cancel);
            int n = pmbx->exec();
            if (n == QMessageBox::Cancel)
                return;
            if (n == QMessageBox::Yes)
            {
                slotSave();
                m_tbw->removeTab(0);
            }
            if (n == QMessageBox::No)
            {
                m_tbw->removeTab(0);
            }
        }
        else m_tbw->removeTab(0);
        m_StringList.removeAt(0);
        m_VectorFlagsChanged.erase(m_VectorFlagsChanged.begin());
        m_StringListModel->setStringList(m_StringList);
    }
    m_tbw->setWindowTitle("TextEditor");
}

void TextEditor::slotExit()
{
    slotCloseAll();
    close();
}

void TextEditor::slotCut()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    emit TE->cut();
}

void TextEditor::slotCopy()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    emit TE->copy();
}
void TextEditor::slotPaste()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    emit TE->paste();
}
void TextEditor::slotDelete()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    //emit TE->delete();
}
void TextEditor::slotSelectAll()
{
    QTextEdit* TE = (QTextEdit*)m_tbw->currentWidget();
    emit TE->selectAll();
}

void TextEditor::slotViewExplorer(bool flag)
{
    Explorer->setVisible(flag);
}
void TextEditor::slotViewExplorerOpenDocuments(bool flag)
{
    ExplorerOpenDocs->setVisible(flag);
}


void TextEditor::slotExplorerSetDoc(QModelIndex p_Index)
{
    m_tbw->setCurrentIndex(p_Index.row());
    setWindowTitle(m_StringList.at(p_Index.row()));
}

void TextEditor::slotSetCurrentDoc(int p_Index)
{
    qDebug() << "SetCurrent";
    setWindowTitle(m_StringList.at(p_Index));
}

void TextEditor::slotCloseTab(int p_Ind)
{
    if(m_VectorFlagsChanged[p_Ind] == true)
    {
        qDebug() << "1";
        QMessageBox* pmbx =
         new QMessageBox(QMessageBox::Information,
         "MessageBox",
         "Сохранить изменения файла?",
         QMessageBox::Yes | QMessageBox::No |
         QMessageBox::Cancel);
        int n = pmbx->exec();
        if (n == QMessageBox::Yes)
        {
            slotSave();
            m_tbw->removeTab(p_Ind);
        }
        if (n == QMessageBox::No)
        {
            m_tbw->removeTab(p_Ind);
        }
    }
    else m_tbw->removeTab(p_Ind);
    m_StringList.removeAt(p_Ind);
    m_StringListModel->setStringList(m_StringList);
    m_VectorFlagsChanged.erase(m_VectorFlagsChanged.begin() + p_Ind);

}

void TextEditor::slotSelectCPP()
{
    QTextEdit* TE = (QTextEdit*)(m_tbw->currentWidget());
    new CPPHighlighter("CPPSyntax.xml", TE->document());
}

void TextEditor::dropEvent(QDropEvent *pe)
{
    QList<QUrl> urlList = pe->mimeData()->urls();
    foreach(QUrl url, urlList) {
            QTextEdit* TE = new QTextEdit;
            QString path = url.path();
            path.remove(0, 1);
            QFile file(path);
            QString str;
            QString Text;
            QTextStream TS(&file);
            if (file.open(QIODevice::ReadOnly))
            {
                while(!TS.atEnd())
                {
                   TS.readLineInto(&str);
                   str.append("\n");
                   Text.append(str);
                }
            }
            TE->setText(Text);
            QFileInfo fInfo(file);
            m_tbw->addTab(TE, fInfo.baseName() + "." + fInfo.completeSuffix());
            setWindowTitle(fInfo.baseName() + "." + fInfo.completeSuffix());
            m_StringList.push_back(fInfo.baseName());
            m_StringListModel->setStringList(m_StringList);
            if((fInfo.completeSuffix() == "h") || ((fInfo.completeSuffix() == "cpp")))
            {
                qDebug() << "OK";
                new Highlighter("cppHighlight.xml", TE->document());
            }
            file.close();
    }
}

void TextEditor::closeEvent(QCloseEvent*)
{
    slotCloseAll();
}

void TextEditor::slotAddSyntax()
{
    m_FormSyntax->show();
}

void TextEditor::slotSelectSyntax()
{
    QTextEdit* TE = (QTextEdit*)(m_tbw->currentWidget());
    new Highlighter("UserSyntax.xml", TE->document());
}

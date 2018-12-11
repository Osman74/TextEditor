#ifndef FORMSYNTAX_H
#define FORMSYNTAX_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QStringList>
#include <QGridLayout>
#include <QColor>
#include <QColorDialog>
#include <QPushButton>
#include <QLabel>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QDomAttr>
#include <QString>
#include <QDebug>
#include <cstdlib>
class FormSyntax : public QWidget
{
    Q_OBJECT
    QLabel* m_Hint;
    QTextEdit* m_TE;
    QStringList m_KeyWords;
    QGridLayout* m_Layout;
    QColor m_Color;
    QPushButton* m_SelectColor;
    QPushButton* m_OK;
public:
    explicit FormSyntax(QWidget *parent = nullptr);
    void createXML();
    QDomElement keyWord(QDomElement& domEl, QDomDocument& domDoc, const QString& keyWord);
    QDomElement Color(QDomElement& domEl, QDomDocument& domDoc, const QString& Color);
    QDomElement makeElement(QDomDocument& domDoc, const QString& strName, const QString& strAttr=QString(),
                            const QString&strText =QString());
    virtual void closeEvent(QCloseEvent*);
signals:
    void signalClose();

public slots:
    void slotSelectColor();
    void slotOK();
};

#endif // FORMSYNTAX_H



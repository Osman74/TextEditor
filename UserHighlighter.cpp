#include "UserHighlighter.h"
#include <string>
#include <QColor>

void UserHighlighter::traverseNode(const QDomNode& node, QStringList& keywordPatterns)
{
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull()) {
    if(domNode.isElement()) {
        QDomElement domElement = domNode.toElement();
        if(!domElement.isNull()) {
            if(domElement.tagName() == "language") {
               qDebug() << "Lang: "
               << domElement.attribute("name", "");
            }
            else if(domElement.tagName() == "keyword"){
                qDebug() << "Keyword: " << domElement.text();
                keywordPatterns << domElement.text();
            }
            else
            {
                qDebug() << "Color: " << domElement.text();
                char c1[3];
                c1[0]= domElement.text().toStdString().c_str()[0];
                c1[1]= domElement.text().toStdString().c_str()[1];
                c1[2]='\0';
                char c2[3];
                c2[0]= domElement.text().toStdString().c_str()[2];
                c2[1]= domElement.text().toStdString().c_str()[3];
                c2[2]='\0';
                char c3[3];
                c3[0]= domElement.text().toStdString().c_str()[4];
                c3[1]= domElement.text().toStdString().c_str()[5];
                c3[2]='\0';
                int r =strtol(c1,nullptr, 16);
                int g =strtol(c2,nullptr, 16);
                int b =strtol(c3,nullptr, 16);
                keywordFormat.setForeground(QBrush(QColor(r,g,b)));
            }
        }
    }
    traverseNode(domNode, keywordPatterns);
    domNode = domNode.nextSibling();
    }
}

UserHighlighter::UserHighlighter(QString XMLFilename, QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    //keywordFormat.setForeground(Qt::darkBlue);
    //keywordFormat.setFontWeight(Qt::cyan);
    QStringList keywordPatterns;

    QDomDocument domDoc;
    QFile file(XMLFilename);
        if(file.open(QIODevice::ReadOnly)) {
            qDebug() << "File " +XMLFilename + "Open";
            if(domDoc.setContent(&file)) {
                qDebug() << "ok";
                QDomElement domElement= domDoc.documentElement();
                traverseNode(domElement, keywordPatterns);
            }
            else qDebug() << "errorDomSet";
        file.close();
     }
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
}

void UserHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);
}

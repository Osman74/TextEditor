#include "Highlighter.h"
#include <string>
#include <QColor>

void Highlighter::traverseNode(const QDomNode& node, QStringList& keywordPatterns, QString& fileExt, bool flag)
{

    QDomNode domNode;
    if (flag) domNode= node.firstChild();
    else domNode = node;
    while(!domNode.isNull()) {
    if(domNode.isElement()) {
        QDomElement domElement = domNode.toElement();
        if(!domElement.isNull()) {
            if(domElement.tagName() == "language") {
               qDebug() << "Lang: "
               << domElement.attribute("name", "") << domElement.attribute("ext", "");
               QStringList extList = domElement.attribute("ext", "").split(" ");
               if(!extList.contains(fileExt))
               {
                   domNode = domNode.nextSibling();
                   qDebug() << "OK";
                   flag = false;
               }
               else
                   flag = true;
            }
            else if(domElement.tagName() == "keyword"){
                qDebug() << "Keyword: " << domElement.text();
                keywordPatterns << domElement.text();
            }
            else
            {
//                qDebug() << "Color: " << domElement.text();
//                char c1[3];
//                c1[0]= domElement.text().toStdString().c_str()[0];
//                c1[1]= domElement.text().toStdString().c_str()[1];
//                c1[2]='\0';
//                char c2[3];
//                c2[0]= domElement.text().toStdString().c_str()[2];
//                c2[1]= domElement.text().toStdString().c_str()[3];
//                c2[2]='\0';
//                char c3[3];
//                c3[0]= domElement.text().toStdString().c_str()[4];
//                c3[1]= domElement.text().toStdString().c_str()[5];
//                c3[2]='\0';
//                int r =strtol(c1,nullptr, 16);
//                int g =strtol(c2,nullptr, 16);
//                int b =strtol(c3,nullptr, 16);
                keywordFormat.setForeground(QBrush(QColor(domElement.text())));
            }
        }
    }
    traverseNode(domNode, keywordPatterns, fileExt, flag);
    if(flag) domNode = domNode.nextSibling();
    }
    qDebug() << "c";
}

Highlighter::Highlighter(QString XMLFilename, QString fileExt, QTextDocument *parent)
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
                traverseNode(domElement, keywordPatterns, fileExt);
            }
            else qDebug() << "errorDomSet";
        file.close();
     }
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);


    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::gray);


    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);


    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);


    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void Highlighter::highlightBlock(const QString &text)
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


    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}

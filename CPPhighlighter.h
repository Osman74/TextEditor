#ifndef CPPHIGHLIGHTER_H
#define CPPHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QtXml/QtXml>
#include <cstdlib>

class CPPHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
public:
    CPPHighlighter(QString XMLFilename, QTextDocument *parent = 0);
    void traverseNode(const QDomNode& node, QStringList& keywordPatterns);

protected:
    void highlightBlock(const QString &text) override;


private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};
#endif // CPPHIGHLIGHTER_H

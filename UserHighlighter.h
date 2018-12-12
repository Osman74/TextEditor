#ifndef USERUserHighlighter_H
#define USERUserHighlighter_H

#include <QSyntaxHighlighter>
#include <QtXml/QtXml>
#include <cstdlib>

class UserHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    UserHighlighter(QString XMLFilename, QTextDocument *parent = 0);
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

#endif // Highlighter_H

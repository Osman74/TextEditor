#include "formsyntax.h"
FormSyntax::FormSyntax(QWidget *parent) : QWidget(parent)
{
    m_Hint = new QLabel("Введите ключевые слова");
    m_TE = new QTextEdit;
    m_SelectColor = new QPushButton("Select color");
    m_OK = new QPushButton("OK");
    connect(m_SelectColor, SIGNAL(clicked()), this, SLOT(slotSelectColor()));
    connect(m_OK, SIGNAL(clicked()), this, SLOT(slotOK()));
    m_Layout = new QGridLayout();
    m_Layout->addWidget(m_Hint);
    m_Layout->addWidget(m_TE);
    m_Layout->addWidget(m_SelectColor);
    m_Layout->addWidget(m_OK);
    setLayout(m_Layout);
}

void FormSyntax::slotSelectColor()
{
    m_Color = QColorDialog::getColor(Qt::blue, this);
}

void FormSyntax::createXML()
{
    m_KeyWords = m_TE->toPlainText().split(" ");
    QDomDocument doc ("languages");
    QDomElement domElement = doc.createElement("languages");
    QDomElement domEl = makeElement(doc,
                                        "lang",
                                        "UserSyntax");
    doc.appendChild(domElement);
    for(auto& str : m_KeyWords)
    {
        QDomElement langl = keyWord(domEl, doc, str);
        domElement.appendChild(langl);
    }
    int r, g, b;
    r = m_Color.red();
    g = m_Color.green();
    b = m_Color.blue();
    char c1[3];
    char c2[3];
    char c3[3];
    itoa(r, c1, 16);
    itoa(g, c2, 16);
    itoa(b, c3, 16);
    char HEX[7];

    HEX[0] = c1[0];
    HEX[1] = c1[1];
    HEX[2] = c2[0];
    HEX[3] = c2[1];
    HEX[4] = c3[0];
    HEX[5] = c3[1];
    HEX[6] = '\0';
    for(int i = 0; i< 6; ++i){
        if(HEX[i]=='\0')
            HEX[i]='0';
    }
    QDomElement color = Color(domEl, doc, HEX);
    qDebug() << c1;
    qDebug() << c2;
    qDebug() << c3;
    qDebug() << HEX;
    domElement.appendChild(color);
    QFile file ("UserSyntax.xml");
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream(&file) << doc.toString();
        file.close();
    }
}

QDomElement FormSyntax::makeElement(QDomDocument& domDoc, const QString& strName, const QString& strAttr,
                                                                            const QString& strText)
{
    QDomElement domElement = domDoc.createElement(strName);
    if (!strAttr.isEmpty()) {
        QDomAttr domAttr = domDoc.createAttribute("name");
        domAttr.setValue(strAttr);
        domElement.setAttributeNode(domAttr);
    }
    if ( !strText. isEmpty ())
    {
        QDomText domText = domDoc.createTextNode(strText);
        domElement.appendChild(domText);
    }
    return domElement;
}


QDomElement FormSyntax::keyWord(QDomElement& domEl, QDomDocument& domDoc, const QString& keyWord)
{
    domEl.appendChild(makeElement(domDoc, "keyword", "", keyWord));
    return domEl;
}
QDomElement FormSyntax::Color(QDomElement& domEl, QDomDocument& domDoc, const QString& color)
{
    domEl.appendChild(makeElement(domDoc, "color", "", color));
    return domEl;
}

void FormSyntax::slotOK()
{
    createXML();
    close();
}

void FormSyntax::closeEvent(QCloseEvent* p_event)
{
    emit signalClose();
}

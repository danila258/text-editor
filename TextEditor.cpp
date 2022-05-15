#include "TextEditor.h"

TextEditor::TextEditor(QWidget* parent) : QWidget(parent)
{
    setWindowTitle("Text Editor");
    resize(500, 500);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    QVBoxLayout* toolsLayout = new QVBoxLayout();
    QVBoxLayout* wordDeleteLayout = new QVBoxLayout();
    _dynamicButtonsLayout = new QVBoxLayout();

    _textInputField = new QTextEdit();
    connect(_textInputField, SIGNAL(textChanged()), SLOT(userInputParser()));

    QPushButton* calcButton1 = new QPushButton("1");
    QPushButton* calcButton2 = new QPushButton("2");

    mainLayout->addWidget(_textInputField);
    _dynamicButtonsLayout->addWidget(calcButton1);
    wordDeleteLayout->addWidget(calcButton2);

    toolsLayout->addLayout(_dynamicButtonsLayout);
    toolsLayout->addLayout(wordDeleteLayout);
    mainLayout->addLayout(toolsLayout);

    setLayout(mainLayout);
}

void TextEditor::userInputParser()
{
    QString text = _textInputField->toPlainText();
    QString word;
    int startPos = 1;

    if ( text.isEmpty() )
    {
        dynamicButtonsUpdate(" ");
        return;
    }

    if (text[text.size() - 1] == ' ')
    {
        startPos = 2;
    }

    for (int i = text.size() - startPos; i > -1; --i)
    {
        if (i == ' ')
        {
            break;
        }

        word += text[i];
    }

    std::reverse(word.begin(), word.end());
    word = "danila";

    if (text[text.size() - 1] == ' ')
    {
        if (word.size() > 1)
        {
            qDebug() << "insert to dictionary";
            _wordsDictionary.insert(word.toStdString());
            dynamicButtonsUpdate(" ");
        }
    }
    else
    {
        qDebug() << "not insert";
        dynamicButtonsUpdate(word);
    }
}

void TextEditor::dynamicButtonsUpdate(const QString& word)
{
    for(int i = 0; i < _dynamicButtonsLayout->count(); ++i)
    {
        QPushButton* button = static_cast<QPushButton*>(_dynamicButtonsLayout->itemAt(i)->widget());
        delete button;
    }


    if (word == " ")
    {
        return;
    }

    std::vector<std::string> suitableWords = _wordsDictionary.findBestMatches(word.toStdString(), 5);

    qDebug() << "update buttons";
    if (suitableWords.size() > 0)
        qDebug() << QString::fromStdString(suitableWords[0]);

    for (int i = 0; i < suitableWords.size(); ++i)
    {
        QPushButton* button = createDynamicButton(QString::fromStdString(suitableWords[i]));
        _dynamicButtonsLayout->addWidget(button);
    }
}

QPushButton* TextEditor::createDynamicButton(const QString& word)
{
    qDebug() << "create buttons";
    QPushButton* button = new QPushButton(word);
    connect(button, SIGNAL(clicked()), SLOT(dynamicButtonClicked()));

    return button;
}

void TextEditor::dynamicButtonClicked() {
    QString word = ((QPushButton*) sender())->text();
    QString text = _textInputField->toPlainText() + word;

    _textInputField->setText(text);
}
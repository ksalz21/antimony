#include <Python.h>

#include <QTextCursor>
#include <QPlainTextEdit>
#include <QGraphicsTextItem>

#include "undo/undo_change_script.h"

#include "graph/script_node.h"

UndoChangeScript::UndoChangeScript(ScriptNode* n, QPlainTextEdit* editor)
    : node(n), editor(editor)
{
    setText("'change script'");

    auto doc = editor->document();
    doc->undo();
    before = doc->toPlainText();
    cursor_before = editor->textCursor().position();

    doc->redo();
    after = doc->toPlainText();
    cursor_after = editor->textCursor().position();
}

void UndoChangeScript::setCursor(int pos)
{
    if (editor)
    {
        QTextCursor c = editor->textCursor();
        c.setPosition(pos);
        editor->setTextCursor(c);
    }
}

void UndoChangeScript::redo()
{
    node->setScript(after.toStdString());
    setCursor(cursor_after);
}

void UndoChangeScript::undo()
{
    // Save text value and cursor position
    after = QString::fromStdString(node->getScript());
    if (editor)
        cursor_after = editor->textCursor().position();

    // Update the script's text and restore the cursor
    node->setScript(before.toStdString());
    setCursor(cursor_before);
}

void UndoChangeScript::swapPointer(Node* a, Node* b) const
{
    if (node == a)
    {
        assert(dynamic_cast<ScriptNode*>(b));
        node = static_cast<ScriptNode*>(b);
    }
}

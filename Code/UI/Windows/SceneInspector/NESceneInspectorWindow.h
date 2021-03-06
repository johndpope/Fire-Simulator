#pragma once


class QTreeWidgetItem;

class NENode;

namespace Ui {
class NESceneInspectorWindow;
}

class NESceneInspectorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NESceneInspectorWindow(QWidget *parent = 0);
    ~NESceneInspectorWindow();
public slots:
    void removeSelectedItem();
    //! Sets the root node for the scene inspector
public:
    void setRoot(QObject* root);
    void update();
    void setNewCurrentItem(QObject* obj);
    void setCurrentItems(QList<NENode*> objList);
    bool isSelectedObject(NENode* obj);
    void setParentCurrentItem();
    void writeGUIState(QXmlStreamWriter *writer);
    void readGUIState(QXmlStreamReader *reader);

protected slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void SetChildsVisibility(QTreeWidgetItem * item, QString exp);

private:
    Ui::NESceneInspectorWindow *ui;
};

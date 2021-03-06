#pragma once

NE_FWD_DECL_6(class, NEGUIManager, NEDockWidget, NETimeline, QSplashScreen, NEAboutDialog, NEVisWindow)
NE_FWD_DECL_3(class, NESceneLoadingDialog, NE_PRIVATE(NEMainWindow), NEMenu)
NE_FWD_DECL_5(class, NEPropertyEditorWindow, NESceneInspectorWindow, NESystemInspectorWindow, NEDockWidget, NEMessageWindow)
NE_FWD_DECL_3(class, FZoneInspectorWindow, NEBufferVisWindow, NESceneBuilderWindow)
NE_FWD_DECL_1(class, NEShelfBar)

/*! \brief NEMainWindow class is responsible for initializing the neutrino main window.
*/

namespace Ui
{
class GUI;
}

class NECEXP_NEUI NEMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	NEMainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
	virtual ~NEMainWindow();
	void setWindowTitle(QString scenePath);
	void updateWindowVisibilityFlags();
	NESceneLoadingDialog* sceneLoadingWidget() const;
	NETimeline* Timeline() const;
	NEShelfBar* shelfBar();
	NEMenu* creationMenu(uint nodeType);
	NEPropertyEditorWindow* propertyEditorWindow();
	NESceneInspectorWindow* sceneInspectorWindow();
	NESystemInspectorWindow* systemInspectorWindow();
    FZoneInspectorWindow* fZoneInspectorWindow();
	NESceneBuilderWindow* sceneBuilder();
	NEMessageWindow* messageWindow();
	NEDockWidget* messageDock();
	NEDockWidget* sceneInspectorDock();
	NEDockWidget* propertyEditorDock();
	void registerRemoveAction(QAction* a);

	//! Locks the docks in the given area
	void lockDocks(Qt::DockWidgetArea area);
	//! Unlocks the docks in the given area
	void unlockDocks(Qt::DockWidgetArea area);

public slots:
	bool newScene();
	void open(QString filePath = QString());
	void openRecentFile();
	bool save();
	bool saveAs();
	bool archive();
	void dropEvent(QDropEvent* event);
	bool import();
    bool importFRANX();
	bool exportAll();
	bool exportSelected();
	bool import(QString filePath);
	bool reference();
	bool reference(QString filePath);
	bool exportAll(QString filePath);
	bool exportSelected(QString filePath);
	void about();
	//! Exports the current frame data with the scene file as a new scene
	bool exportCurrentFrame();
	//! Renders the current frame with the specified external ray tracer / renderer.
	void renderCurrentFrame();
	//! Transforms the sequence (min to max frame) into a format readable by the specified external ray tracer / renderer.
	void renderSequence();
	void renderDeleteSequence();
	void showSceneManagementDialog();
	void showSceneLoadingWidget();
	//! This is somehow required on mac to be able open scene files through finder
	void showSceneLoadingSplash(QString filename);
	void hideSceneLoadingSplash();
	void hideEverythingForStartup();
	//! Hides all windows
	void hideAllWindows();
	//! Shows all windows
	void showAllWindows();
	//! For maximizing the OpenGL window under cursor using spacebar
	void maximizeGLWindowUnderCursor();
    //! Show Main GL windows
    void showGlWindow();

signals:
    uint importFRANX( QString );

protected:
	//! Hanled when the user closes the windows
	void closeEvent(QCloseEvent* event);
	//! Handles when the user presses a key
	void keyPressEvent(QKeyEvent* event);
	//! Handled when the user resizes the main window
	void resizeEvent(QResizeEvent* event);

private slots:
	void resizeTimeout();
	void on_actionDuplicate_triggered();

private:
    Ui::GUI* ui;
	NE_DECL_PRIVATE(NEMainWindow)

};

#include <QtGui>

#include "view.h"
#include "visualizer.h"
#include "gameitem.h"

Visualizer::Visualizer(NKBManager* kbManager, QWidget *parent) :
    QWidget(parent)
  ,m_PosCount(0)
  ,m_ColCount(0)
  ,m_KBManager(kbManager)
{
	m_FactoryRect.setRect(-70, 0, 60, 60);

	QColor col(100, 100, 100, 50);
	QRectF rect(0, 0, 600, 600);
	m_Scene = new GameScene(this);
	m_Scene->CreateGrid(10, rect, col);

	View *view = new View("GameFieldView");
	view->view()->setScene(m_Scene);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(view);
	setLayout(layout);
}

void Visualizer::populateScene()
{
 	// Получаем домен "Команда"
	QVector<QString> teams = m_KBManager->GetDomainValsByString("Команды");

	// Получаем фреймы изображений всех типов персонажей
	NFrame* imgFrame = m_KBManager->GetFrameByName("Изображение персонажа");
	QList<NFrame*> imgChildren = m_KBManager->GetFrameChildren(imgFrame);
	
	for (int i = 0; i < teams.count(); i++)
	{
		QColor& color = GetNextColor();
		for (int j = 0; j < imgChildren.count(); j++)
		{
			NFrame* frame = imgChildren.at(j);
			NSlot* picSlot = m_KBManager->GetFrameSlot(frame, "Картинка");
			NSlot* typeSlot = m_KBManager->GetFrameSlot(frame, "Тип");
			if (picSlot != NULL && typeSlot != NULL)
			{
				QPixmap pic(picSlot->defValue().toString());
				QString type = typeSlot->defValue().toString();
				m_Scene->CreateFactory(new GameItem(type, pic, teams.at(i), color), GetNextPos());
			}
		}
	}

	imgFrame = m_KBManager->GetFrameByName("Изображение");
	imgChildren = m_KBManager->GetFrameChildren(imgFrame);

	for (int j = 0; j < imgChildren.count(); j++)
	{
		NFrame* frame = imgChildren.at(j);
		if (frame->name.defValue() != "Изображение персонажа");

		NSlot* picSlot = m_KBManager->GetFrameSlot(frame, "Картинка");
		NSlot* typeSlot = m_KBManager->GetFrameSlot(frame, "Тип");
		if (picSlot != NULL && typeSlot != NULL)
		{
			QPixmap pic(picSlot->defValue().toString());
			QString type = typeSlot->defValue().toString();
			m_Scene->CreateFactory(new GameItem(type, pic, "", QColor()), GetNextPos());
		}
	}
}

void Visualizer::Update()
{

}

Grid* Visualizer::GetGrid()
{
    return m_Scene->GetGrid();
}

QRectF& Visualizer::GetNextPos()
{
	QRectF* rect = new QRectF();
	rect->setRect
		(
			m_FactoryRect.x(), 
			m_FactoryRect.y() + 70 * m_PosCount, 
			m_FactoryRect.width(), 
			m_FactoryRect.height()
		);
	m_PosCount++;
	return *rect;
}

QColor& Visualizer::GetNextColor()
{
	QColor* color = new QColor();
	color->setRgb
		(
			255 - m_ColCount * 100,
			m_ColCount * 10,
			m_ColCount * 100,
			150
		);
	m_ColCount++;
	return *color;
}

void Visualizer::RedrawItems()
{
	m_PosCount = 0;
	m_ColCount = 0;
	m_Scene->Clear();
	populateScene();
}
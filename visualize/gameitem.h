#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QtGui>

enum GameItemType
{
    gitWarior,
    gitHealer,
    gitArcher,
    gitMeat
};

enum GameTeam
{
    gtRed,
    gtBlue,
    gtNull
};

class GameItem;

class GameMimeData : public QMimeData
{
private:
    GameItem* m_Item;

public:
    GameMimeData();
    GameItem* GetItem();
    void SetItem(GameItem* item);
};

class GameItem
{
protected:
    QPixmap         m_Pic;
	QColor			m_Color;
    QString         m_Team;
	QString			m_Type;
    int				m_FrameId;
	int				m_Bright;

public:
    GameItem(QString type, QPixmap& pic, QString team, QColor color, int id = 0);
	QString GetType() {return m_Type;}
	QPixmap& GetPic() {return m_Pic;}
	QString GetTeam() {return m_Team;}
	QColor& GetColor() {return m_Color;}
	int GetBright() {return m_Bright;}

	int GetFrameId() {return m_FrameId;}
	void SetFrameId(int id) {m_FrameId = id;}

	void UpdatePic(QString path);
	void UpdateBrigth(int bright);
};

#endif // GAMEITEM_H

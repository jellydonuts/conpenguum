#include "BaseWindow.h"

#include "../Animation/AnimationEnum.h"

using namespace std;

/*------------------------------------------------------------------------------
 --
 -- CONSTRUCTOR: BaseWindow::BaseWindow()
 --
 -- DESIGNER: Erick Ribeiro
 --
 -- PROGRAMMER: Erick Ribeiro
 --
 -- REVISIONS:
 --
 --		Feb 18, 2010 - Erick Ribeiro
 --		Set up a QTimer object to call timerEvent() every interval.
 --
 --		Mar 09, 2010 - Brad Paugh
 --		Added init of chatting variable and retrieval of CommClient instance.
 --
 --		Mar 15, 2010 - Brad Paugh
 --		Added instantiation of renderer into the constructor, we may not want
 --		to keep it this way, will have to discuss with other members of the project.
 --
 -- NOTES:
 -- This is the main constructor of the BaseWindow class. It initializes
 -- the QTimer object used for timing.
 --
 -----------------------------------------------------------------------------*/
BaseWindow::BaseWindow() :
    frameRate(DEFAULT_FRAME_RATE), timer(this)
{
    for (size_t i = MAX_REAL_OBJECT + 1; i < MAX_REAL_OBJECT + MAX_TRANSIENT_OBJECT; ++i)
        freeIds.push(i);

    connect(&timer, SIGNAL(timeout()), this, SLOT(timerEvent()));
    chatting = false;
    theClient = CommClient::Instance();

    chatIndex = 0;
    shift = false;
    localChat = "";

    //loadMap
    Map* m = new Map();
    QFile mapFile("Resources/theMap");
    if (!mapFile.open(QIODevice::ReadOnly | QIODevice::Text))
        Logger::LogNQuit("Could not open map file");
    QTextStream mapStream(&mapFile);
    while (!mapStream.atEnd())
    {
        int row, col, tileNum;
        string line = mapStream.readLine().toStdString();
        std::istringstream iss(line);
        iss >> row >> col >> tileNum;
        m->addTile(tileNum, row, col);
    }

    /*this may be temporary*/
    this->setFixedSize(1024, 768);
    ren = new Renderer(this, gameState, &chatQueue, &localChat);
    this->show();

    animationMap = Animation::getAnimationMap();
}

/*------------------------------------------------------------------------------
 --
 -- CONSTRUCTOR: BaseWindow::start()
 --
 -- DESIGNER: Brad Paugh
 --
 -- PROGRAMMER: Brad Paugh
 --
 -- REVISIONS:
 --
 --		Mar 9, 2010 Brad Paugh
 --		Currently connecting player with hardcoded name "player"
 --		and to the loopback IP. This could be easily changed once the GUI
 --		items to enter player name and IP are made.
 --
 -- NOTES:
 -- This is the method that is called when a player presses the GUI item
 -- to start the game.
 --
 -----------------------------------------------------------------------------*/
void BaseWindow::Start(int clientId)
{
    clientAction = new ClientAction(clientId);
    startRendering();
}

/*------------------------------------------------------------------------------
 --
 -- METHOD: BaseWindow::keyPressEvent()
 --
 -- DESIGNER: Erick Ribeiro
 --
 -- PROGRAMMER: Brad Paugh, Renz Barron
 --
 -- REVISIONS:
 --
 --             Mar 4, 2010 - Brad Paugh
 --             Added Key press handling to the method. Currently commented out
 --              as we need more info on how the CommClient will be instantiated
 --              and how we get the clientID to instantiate ClientAction.
 --
 --		Mar 9, 2010 - Brad Paugh
 --		Uncommented and added chat handling. clientId temporarily set to 1.
 --
 --		Mar 9, 2010 - Brad Paugh
 --		Now creating a ClientAction with the proper client ID as received from server.
 --
 -- NOTES:
 -- This method handles keyboard input. It's responsibility is to translate key
 -- presses to game actions, and send these actions to the server using the
 -- CommClient object.
 --
 -----------------------------------------------------------------------------*/
void BaseWindow::keyPressEvent(QKeyEvent * event)
{
    /* ClientID will be extracted from server message it is NOT generated by the client.
     get server message of type 5(MT_INIT) and GetClientID() from that message*/

    if (chatting)
    {
        handleChat(event->key());
    }
    else
    {
        switch (event->key())
        {
        case Qt::Key_Left:
            clientAction->setTurningLeft();
            break;
        case Qt::Key_Right:
            clientAction->setTurningRight();
            break;
        case Qt::Key_Up:
            clientAction->setAccelerating();
            break;
        case Qt::Key_Down:
            clientAction->setDecelerating();
            break;
        case Qt::Key_Control:
            clientAction->setFiring();
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            toggleChat();
            handleChat('>');
            handleChat(' ');
            break;
        }
    }
}
void BaseWindow::keyReleaseEvent(QKeyEvent * event)
{
    /* ClientID will be extracted from server message it is NOT generated by the client.
     get server message of type 5(MT_INIT) and GetClientID() from that message*/

    switch (event->key())
    {
    case Qt::Key_Left:
        clientAction->unsetTurningLeft();
        break;
    case Qt::Key_Right:
        clientAction->unsetTurningRight();
        break;
    case Qt::Key_Up:
        clientAction->unsetAccelerating();
        break;
    case Qt::Key_Down:
        clientAction->unsetDecelerating();
        break;
    case Qt::Key_Shift:
        shift = false;
        break;
    case Qt::Key_Control:
        clientAction->unsetFiring();
        break;
    }
}

/*------------------------------------------------------------------------------
 --
 -- METHOD: BaseWindow::handleChat(int key)
 --
 -- DESIGNER: Brad Paugh, Renz Barron
 --
 -- PROGRAMMER: Brad Paugh, Renz Barron
 --
 -- REVISIONS:
 --
 -- NOTES:
 --  This method handles the chat string depending on which
 --  key was pressed. (Qt:key value is passed in) returns -1 if
 --  a non displayable character is passed in and 0 if a
 --  displayable/handled character was passed in. The renderer will
 --  simply retreive this string and display it.
 --
 -----------------------------------------------------------------------------*/
int BaseWindow::handleChat(int key)
{
	//if enter send the message
	if ((key == Qt::Key_Enter) || (key == Qt::Key_Return))
	{
		chatString = chatString.substr(2);
		if (chatString.size() > 0)
			theClient->sendServerMsg(chatString);
		chatString.clear();
		toggleChat();
	}
	//if shift set shift on
	else if (key == Qt::Key_Shift)
	{
		shift = true;
		return 0;
	}
	//if backspace erase the last character
	else if (key == Qt::Key_Backspace)
	{
		if (chatString.length() != 2)
			chatString.erase(chatString.length()-1, 1);
	}
	else
	{
		//if its a displayable character
		if ((key >= 32) && (key <= 127))
		{
			if (chatString.length() < 57)
			{
				if (!shift && (char(key) >= 65) && (char(key) <= 90))
					chatString.push_back(char(key+32));
				else
					chatString.push_back(char(key));
			}
		}
		else
		{
			//not a displayable character
			return -1;
		}
	}
	localChat = chatString.c_str();
	return 0;
}

/*------------------------------------------------------------------------------
 --
 -- METHOD: BaseWindow::toggleChat()
 --
 -- DESIGNER: Brad Paugh
 --
 -- PROGRAMMER: Brad Paugh
 --
 -- REVISIONS:
 --
 -- NOTES:
 --  This method simply toggles whether we are chatting or not chatting.
 --
 -----------------------------------------------------------------------------*/
void BaseWindow::toggleChat()
{
    chatting = !chatting;
}

/*------------------------------------------------------------------------------
 --
 -- METHOD: BaseWindow::getChatString()
 --
 -- DESIGNER: Brad Paugh
 --
 -- PROGRAMMER: Brad Paugh
 --
 -- REVISIONS:
 --
 -- NOTES:
 --  Getter for the ChatString
 --
 -----------------------------------------------------------------------------*/
std::string BaseWindow::getChatString()
{
    return chatString;
}

/*------------------------------------------------------------------------------
 --
 -- METHOD: BaseWindow::startRendering()
 --
 -- DESIGNER: Erick Ribeiro
 --
 -- PROGRAMMER: Erick Ribeiro
 --
 -- REVISIONS:
 --
 -- NOTES:
 -- Call this method to start the timer mechanism resposible for calling
 -- render() for every frame.
 --
 -----------------------------------------------------------------------------*/
void BaseWindow::startRendering()
{
    timer.start(1000 / frameRate);
}

/*------------------------------------------------------------------------------
 --
 -- METHOD: BaseWindow::stopRendering()
 --
 -- DESIGNER: Erick Ribeiro
 --
 -- PROGRAMMER: Erick Ribeiro
 --
 -- REVISIONS:
 --
 -- NOTES:
 -- Call this method to stop the timer mechanism resposible for calling
 -- render() for every frame.
 --
 -----------------------------------------------------------------------------*/
void BaseWindow::stopRendering()
{
    timer.stop();
}

/*------------------------------------------------------------------------------
 --
 -- METHOD: BaseWindow::setFrameRate(int rate)
 --
 -- DESIGNER: Erick Ribeiro
 --
 -- PROGRAMMER: Erick Ribeiro
 --
 -- REVISIONS:
 --
 -- PARAMETERS:
 --		int rate - The amount of frames per second.
 --
 -- NOTES:
 -- This method updates the frame rate used for calling render().
 --
 -----------------------------------------------------------------------------*/
void BaseWindow::setFrameRate(int rate)
{
    frameRate = rate;
    timer.setInterval(1000 / rate);
}

/*------------------------------------------------------------------------------
 --
 -- METHOD: BaseWindow::updateGameState()
 --
 -- DESIGNER: Erick Ribeiro & John Kuo
 --
 -- PROGRAMMER: Erick Ribeiro & John Kuo
 --
 -- REVISIONS:
 --
 -- NOTES:
 -- This method updates the game state.
 --
 -----------------------------------------------------------------------------*/
void BaseWindow::updateGameState()
{
    while (theClient->hasNextUpdate())
    {
        UpdateObject updateObj = theClient->nextUpdate();
        int objId = updateObj.getObjectId();

        if (objId == clientAction->getObjectId()) //Update position of our ship
            scrnCenter = updateObj.getPos();

        if (objId < MAX_CLIENTS && updateObj.getActions().isAccelerating()) //for Exhaust trails
        {
    		int trailId = freeIds.front(); //Get an ID
    		freeIds.pop();
    		createObject(updateObj, trailId);
        }

        if (gameState.find(objId) != gameState.end()) //If it exists
            gameState[objId].Update(updateObj);
        else
            //Create GameObject
            createObject(updateObj, objId);
    }
}

void BaseWindow::createObject(UpdateObject &updateObj, int objId)
{
    GameObject animObj(updateObj, objId);
    ResourceManager *rm = ResourceManager::GetInstance();
    TexturedResourceDefinition *rd;
    vector<Image> *images;

	animObj.animeIndex = 0;

    if (objId < MAX_CLIENTS)
    {
    	rd = (TexturedResourceDefinition*) rm->GetResource(SHIP, 0);
        images = animationMap[SHIP].getAnimationImages();
        animObj.currentAnime = animationMap[SHIP];
        animObj.animeIndex = 5;
        animObj.owner = getName(objId);
    }
    else if (objId < MAX_SHOTS_ID)
    {
    	rd = (TexturedResourceDefinition*) rm->GetResource(SHOT, 0);
        images = animationMap[SHOT].getAnimationImages();
        animObj.currentAnime = animationMap[SHOT];
    }
    else
    {
    	rd = (TexturedResourceDefinition*) rm->GetResource(EXHAUST, 0);
		images = animationMap[EXHAUST].getAnimationImages();
		animObj.currentAnime = animationMap[EXHAUST];

		double radians = DEGTORAD(animObj.angle * 2); //Offset so that it appears behind the ship
		int x = cos(radians) * -(rd->object_width);
		int y = sin(radians) * -(rd->object_height);
		animObj.position.setX(animObj.position.x() + x);
		animObj.position.setY(animObj.position.y() + y);
    }

    animObj.animeImage = &(*images)[animObj.animeIndex];
    animObj.textureName = animObj.animeImage->getLink();
    animObj.objHeight = rd->object_height;
    animObj.objWidth = rd->object_width;

    gameState[animObj.objectId] = animObj;
}

QString BaseWindow::getName(int playerId)
{
    for (size_t i = 0; i < playerList.size(); ++i)
        if (playerList[i].getId() == playerId)
            return QString(playerList[i].getName().c_str());
    return "";
}

void BaseWindow::clearTransientObjects()
{
    list<int> thingsToErase;
    vector<Image> *images = animationMap[EXHAUST].getAnimationImages();
    vector<Image> *shipImages = animationMap[SHIP].getAnimationImages();

    for (map<int, GameObject>::iterator it = gameState.begin(); it != gameState.end(); ++it)
    {
        if (it->first < MAX_REAL_OBJECT)
            thingsToErase.push_back(it->first);

        GameObject *animatedObj = &it->second;

        if (animatedObj->objectId < MAX_CLIENTS) // If it is a ship
        {
        	;
        }
        else
        {
            if (animatedObj->animeIndex < images->size())
                animatedObj->animeImage = &(*images)[animatedObj->animeIndex++];
            else
                thingsToErase.push_back(it->first);
        }
    }

    while (!thingsToErase.empty())
    {
        gameState.erase(thingsToErase.front());
        thingsToErase.pop_front();
    }
}

/*------------------------------------------------------------------------------
 --
 -- METHOD: BaseWindow::timerEvent()
 --
 -- DESIGNER: Erick Ribeiro
 --
 -- PROGRAMMER:
 --
 -- REVISIONS:
 --
 --		Feb 22, 2010 - Erick Ribeiro
 --		Changed the way render() was being called. We don't need to create a
 --		renderer object. We just need to call render(), which will be
 --		implemented by the Graphics Team in a subclass of BaseWindow.
 --
 -- NOTES:
 -- This method is called by a QTimer object for every frame. It's
 -- responsibility is to determine if the game scene needs to be rendered, and
 -- if so, it calls render().
 --
 -----------------------------------------------------------------------------*/
void BaseWindow::timerEvent()
{
    theClient->sendAction(*clientAction);
    getServerMessage();
    updateGameState();
    ren->buildRenderList(scrnCenter);
    ren->Render(clientAction->getObjectId());
    clearTransientObjects();
}

/*------------------------------------------------------------------------------
 --
 -- METHOD: BaseWindow::getServerMessage()
 --
 -- DESIGNER: Brad Paugh
 --
 -- PROGRAMMER: Brad Paugh
 --
 -- REVISIONS:
 --
 -- NOTES:
 -- This method gets a server message from the server. Server messages
 --  are messages that don't have anything to do with game state updates
 --  such as a chat message or an initial message containing the clientID
 --
 -----------------------------------------------------------------------------*/
void BaseWindow::getServerMessage()
{
    while (theClient->hasNextServerMessage())
    {
        ServerMessage sm = theClient->nextServerMessage();
        //init msg
        if (sm.GetMsgType() == ServerMessage::MT_INIT)
        {
            cout << "MT_INIT RECEIVED: ID: " << clientAction->getObjectId() << endl; //TODO: Handle initial score
        }
        else if (sm.GetMsgType() == ServerMessage::MT_SCORES)
        {
            QStringList players = QString(sm.GetData().c_str()).split('|', QString::SkipEmptyParts);
            for (int i = 0; i < players.size(); i++)
                playerList.push_back(Player(players[i].toStdString()));
        }
        //chat msg
        else if (sm.GetMsgType() == ServerMessage::MT_CHAT)
        {
            if (chatIndex == 6)
                chatQueue.pop_back();
			else
				chatIndex++;
			chatQueue.push_front(sm.GetData().c_str());
        }
        else if (sm.GetMsgType() == ServerMessage::MT_SHUTDOWN)
        {
            printf("shutdown");
            QMessageBox::critical(this, tr("Tuxspace"), tr("Server Shutdown"), QMessageBox::Ok, QMessageBox::Ok);
            // TODO: more graceful shutdown
            exit(0);
        }
        else if (sm.GetMsgType() == ServerMessage::MT_DEATH)
        	ren->setDeathTime(150);
    }
}

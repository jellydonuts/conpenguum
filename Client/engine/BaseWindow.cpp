#include "BaseWindow.h"

#include "../../Core/ConfigParser.h"

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
BaseWindow::BaseWindow() : frameRate(DEFAULT_FRAME_RATE), timer(this), gameState(32)
{
	connect(&timer, SIGNAL(timeout()), this, SLOT(timerEvent()));
	chatting = false;
	theClient = CommClient::Instance();
	
	/*this may be temporary*/
	this->setFixedSize(1024, 768);
	ren = new Renderer(this);
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
void BaseWindow::Start()
{
	ConfigParser cp;
	map<string, string> params;

	if (cp.Parse("client.conf", params) && params.find("username") != params.end()
			                            && params.find("server_ip") != params.end()
			                            && params.find("tcp_port") != params.end())
	{
		clientID = theClient->connect(params["username"], params["server_ip"], params["tcp_port"]);
		clientAction = new ClientAction(clientID);
		startRendering();
	}
	else
		cerr << "Invalid configuration file." << endl;

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
void BaseWindow::keyPressEvent (QKeyEvent * event)
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
				toggleChat();
				break;
		}
	}
}
void BaseWindow::keyReleaseEvent (QKeyEvent * event)
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
	if (key == Qt::Key_Enter)
	{
		theClient->sendServerMsg(chatString);
		toggleChat();
	}
	//if backspace erase the last character
	else if (key == Qt::Key_Backspace)
	{
		chatString.erase(chatString.length()-1, 1);
	}
	else
	{
		//if its a displayable character
		if ((key >= 32) && (key <= 127))
		{
			chatString.push_back(char(key));
		}
		else
		{
			//not a displayable character
			return -1;
		}
	}
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
	ren->Render();
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
void BaseWindow::startRendering ()
{
	timer.start(1000/frameRate);
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
void BaseWindow::stopRendering ()
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
void BaseWindow::setFrameRate (int rate)
{
	frameRate = rate;
	timer.setInterval(1000/rate);
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
void BaseWindow::updateGameState ()
{

	while (theClient->hasNextUpdate())
	{
		GameObject * gameObj;
		UpdateObject updateObj = theClient->nextUpdate();
		int objectId = updateObj.getActions().getObjectID();

		if (objectId <= 31)
		{
			gameObj = &gameState.at(objectId);
		}
		else
		{
			gameObj = new GameObject;
			gameObj->animeIndex = 0;
			gameState.push_back(*gameObj);
		}

		gameObj->objectId = objectId;
		gameObj->position = updateObj.getPos();
		gameObj->angle = updateObj.getRotation();		
	}

	for (size_t i = 32; i < gameState.size(); i++)
	{
		GameObject &animatedObj = gameState.at(i);
		std::vector<Image> images = animationMap[animatedObj.objectId].getAnimationImages();

		if (animatedObj.objectId == 32) // Bullets
			continue;

		if (animatedObj.animeIndex < images.size())
		{			
			animatedObj.animeImage = &images[animatedObj.animeIndex];
			animatedObj.animeIndex++;
		}
	}
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
			clientID = sm.GetClientID();
			std::cout << "MT_INIT RECEIVED: ID: " << clientID << endl;
		}
		//chat msg
		else if (sm.GetMsgType() == ServerMessage::MT_CHAT)
		{
			//handle chat message
		}
	}
}



/*------------------------------------------------------------------------------
 --
 -- METHOD: BaseWindow::render()
 --
 -- DESIGNER: Erick Ribeiro
 --
 -- PROGRAMMER: Erick Ribeiro
 --
 -- REVISIONS:
 --
 -- NOTES:
 -- This method is just an empty virtual method. It must be overriden by
 -- subclasses to implement the actual rendering functionality. This method is
 -- called automatically for every frame.
 --
 -----------------------------------------------------------------------------*/
void BaseWindow::render ()
{
}

// VLC_XSPF_Generator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "tinyxml2.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace tinyxml2;

tinyxml2::XMLDocument xmlDoc;

char URLFront[2048];
char URLEnd[2048];

void insertTrack(XMLElement* _pTrackList, const std::string& location, const unsigned& _id) {
	auto _pTrack = xmlDoc.NewElement("track");
	_pTrackList->InsertEndChild(_pTrack);
	
	auto _pLocation = xmlDoc.NewElement("location");
	_pLocation->SetText(location.c_str());
	_pTrack->InsertEndChild(_pLocation);

	auto _pExtension = xmlDoc.NewElement("extension");
	_pExtension->SetAttribute("application","http://www.videolan.org/vlc/playlist/0");
	
	auto _pVlcId = xmlDoc.NewElement("vlc:id");
	_pVlcId->SetText(_id);
	_pExtension->InsertEndChild(_pVlcId);

	_pTrack->InsertEndChild(_pExtension);
}




int main()
{
	std::string listName;
	std::cout << "Please enter the list name: ";
	std::cin >> listName;

	std::string inputURL;
	std::cout << "Please substitute the episode digits with *(s) and enter the URL: ";
	std::cin >> inputURL;
	std::stringstream ssInputURL(inputURL);
	ssInputURL.getline(URLFront, 2048, '*');
	
	unsigned starCount = 1;
	while (ssInputURL.peek() == '*') {
		ssInputURL.get();
		starCount++;
	}

	ssInputURL.getline(URLEnd, 2048);

	//std::cout << URLFront << starCount << URLEnd;

	unsigned episodeNum;
	std::cout << "Please enter total episode numbers: ";
	std::cin >> episodeNum;

	auto pDeclare = xmlDoc.NewDeclaration();
	xmlDoc.InsertFirstChild(pDeclare);

	auto pPlaylist = xmlDoc.NewElement("playlist");
	pPlaylist->SetAttribute("xmlns", "http://xspf.org/ns/0/");
	pPlaylist->SetAttribute("xmlns:vlc", "http://www.videolan.org/vlc/playlist/ns/0/");
	pPlaylist->SetAttribute("version", "1");
	xmlDoc.InsertEndChild(pPlaylist);
	
	auto pTitle = xmlDoc.NewElement("title");
	pTitle->SetText("Playlist");
	pPlaylist->InsertEndChild(pTitle);

	auto pTrackList = xmlDoc.NewElement("trackList"); 
	pPlaylist->InsertEndChild(pTrackList);

	for (unsigned currentEpisode = 0; currentEpisode < episodeNum; currentEpisode++) {
		std::stringstream ssCurrentURL;
		ssCurrentURL << URLFront << std::setw(starCount) << std::setfill('0') << (currentEpisode+1) << URLEnd;
		insertTrack(pTrackList,ssCurrentURL.str(), currentEpisode);
	}
	
	std::string listNameWithExt = listName + ".xspf";
	xmlDoc.SaveFile(listNameWithExt.c_str());
	xmlDoc.Print();
}

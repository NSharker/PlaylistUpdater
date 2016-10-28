#include <iostream>
#include <vector>
#include <string>
#include <fstream> // text file openings
#include <dirent.h> //find all files in current directory
#include <cstdlib>

#include <Windows.h> //Need to move file
#include <WinBase.h> //Need to move File

#include <curl/curl.h>


using namespace std;


// make struct object = filename, file url
struct MusicFile{
    string fileName;
    string fileUrl;
};



int main(){

    mkdir("delete");
    //MoveFile("Playlist.m3u","delete//");
    //fill the string in playlist variable with the name of the playlist, this is to make sure to delete the correct playlist
    string playlist = "";
    cout <<"Deleting: "<< playlist << endl;
    string deletePlaylist = "delete//";
    deletePlaylist += playlist;
    MoveFile(playlist.c_str(),deletePlaylist.c_str());

    //MoveFile("songurl.txt","delete//");
    string delURL = "songurl.txt";
    cout <<"Deleting: "<< delURL << endl;
    string deleteSongURL = "delete//";
    deleteSongURL += delURL;
    MoveFile(delURL.c_str(),deleteSongURL.c_str());

    //MoveFile("songnames.txt","delete//");
    string delSongName = "songnames.txt";
    cout <<"Deleting: "<< delSongName << endl;
    string deleteSongName = "delete//";
    deleteSongName += delSongName;
    MoveFile(delSongName.c_str(),deleteSongName.c_str());

    //download songnames.txt
    //Fill the string in the *songNamesurl with the url of the location of your songnames.txt
    cout << "Download names of the songs." << endl;
    CURL *songNames;
    FILE *fpSongNames;
    CURLcode songNamesres;
    char *songNamesurl = "";
    char SongNamesoutfilename[FILENAME_MAX] = "songnames.txt";
    songNames = curl_easy_init();
    if (songNames)
    {
        fpSongNames = fopen(SongNamesoutfilename,"wb");
        curl_easy_setopt(songNames, CURLOPT_URL, songNamesurl);
        curl_easy_setopt(songNames, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(songNames, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(songNames, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(songNames, CURLOPT_WRITEDATA, fpSongNames);
        songNamesres = curl_easy_perform(songNames);
        curl_easy_cleanup(songNames);
        fclose(fpSongNames);
    }
    //download songurl.txt
    //Fill the string in the *songUrlurl with  the url of the location of your songurl.txt
    cout << "Downloading the locations of the songs." << endl;
    CURL *songUrlFile;
    FILE *fpSongUrl;
    CURLcode songUrlres;
    char *songUrlurl = "";
    char SongUrloutfilename[FILENAME_MAX] = "songurl.txt";
    songUrlFile = curl_easy_init();
    if (songUrlFile)
    {
        fpSongUrl = fopen(SongUrloutfilename,"wb");
        curl_easy_setopt(songUrlFile, CURLOPT_URL, songUrlurl);
        curl_easy_setopt(songUrlFile, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(songUrlFile, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(songUrlFile, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(songUrlFile, CURLOPT_WRITEDATA, fpSongUrl);
        songUrlres = curl_easy_perform(songUrlFile);
        curl_easy_cleanup(songUrlFile);
        fclose(fpSongUrl);
    }
    //download Playlist.m3u
    //Fill the string in *playlisturl with the url of your playlist
    //Fill playlistoutfilename[FILENAME_MAX] to have the name of your playlist
    cout << "Downloading the playlist file." << endl;
    CURL *playlistFile;
    FILE *fpPlaylist;
    CURLcode playlistres;
    char *playlisturl = "";
    char playlistoutfilename[FILENAME_MAX] = "";
    playlistFile = curl_easy_init();
    if (playlistFile)
    {
        fpPlaylist = fopen(playlistoutfilename,"wb");
        curl_easy_setopt(playlistFile, CURLOPT_URL, playlisturl);
        curl_easy_setopt(playlistFile, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(playlistFile, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(playlistFile, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(playlistFile, CURLOPT_WRITEDATA, fpPlaylist);
        playlistres = curl_easy_perform(playlistFile);
        curl_easy_cleanup(playlistFile);
        fclose(fpPlaylist);
    }

    //open file 1 = filename
    ifstream fileSongName;
    fileSongName.open("songnames.txt");

    //open file 2 = url of each song
    ifstream fileSongUrl;
    fileSongUrl.open("songurl.txt");

    string songName;
    string songUrl;
    vector <MusicFile> songList;


    //Loop through the files to add the name and the url of each to a vector
    while(getline(fileSongName, songName))
    {
       getline(fileSongUrl, songUrl);
       MusicFile temp;
       temp.fileName = songName;
       temp.fileUrl = songUrl;

       songList.push_back(temp);
    }

    //Keeps a list of all the music files in the directory
    vector <string> dirList;


    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (".")) != NULL) {
      /* store all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
        //printf ("%s\n", ent->d_name);
        string fname;
        fname = ent->d_name;
        int strlength = fname.length();

        //Check for filename to be longer than 3
        //there are files called . and .. that we should skip
        //if the file is a .mp3 we add it to music file vector, dirList
        if (strlength > 3)
        {
            string fileType;
            fileType = fname.substr(strlength-3,3);
            if (fileType == "mp3")
            {
                dirList.push_back(fname);
            }

        }


      }
      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
    }




    //search for directory for file name of songs,
        //if found do nothing
        //if not found delete file


    for (int i = 0; i < dirList.size(); i++)
    {
        bool toDelete = true;
        for (int j = 0; j < songList.size(); j++)
        {
                if(dirList[i] == songList[j].fileName)
                {
                    toDelete = false;
                }
        }
        if(toDelete)
        {
            cout <<"Deleting: "<< dirList[i] << endl;
            string folderLocation = "delete//";
            folderLocation += dirList[i];
            MoveFile(dirList[i].c_str(),folderLocation.c_str());

        }
    }


    //search filename for songs in directory
        //if found in directory do nothing
        //if song not found in directory download

    for (int i = 0; i < songList.size(); i++)
    {
        bool toDownload = true;
        for (int j = 0; j < dirList.size();  j++)
        {
                if(dirList[j] == songList[i].fileName)
                {
                    toDownload = false;
                }
        }
        if(toDownload)
        {
            cout <<"Downloading: "<< songList[i].fileName << endl;

            CURL *curl;
            FILE *fp;
            CURLcode res;
            string urlLoc = songList[i].fileUrl;

            char *url = &urlLoc[0];

            string textname = songList[i].fileName;

            char *outfilename = &textname[0];

            curl = curl_easy_init();
            if (curl)
            {
                fp = fopen(outfilename,"wb");
                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                fclose(fp);
            }

        }
    }



    return 0;


}

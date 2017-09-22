//
// Created by azrael on 11/09/17.
//

#include "gtest/gtest.h"
#include "../../Controller/MediaController.h"

class ControllerSuite : public ::testing::Test{
public:
    Playlist* p;
    MediaController* m;
    Song* s1;
    Song* s2;
    Song* s3;
    void SetUp() override{
        p = new Playlist;
        wxBitmap noArt;
        noArt.LoadFile(wxT("/home/azrael/CLionProjects/MediaPlayer/ControlsPNG/NoAlbumArt.png"));
        s1 = new Song(wxT("Title1"), noArt,  wxT("Unknown"), wxT("Unknown"), 0, wxT("/home/azrael/Music/Honeymoon.mp3"));
        s2 = new Song(wxT("Title2"),noArt, wxT("Unknown"), wxT("Unknown"), 0, wxT("/home/azrael/Music/Whisper.mp3"));
        s3 = new Song(wxT("Title3"),noArt, wxT("Unknown"), wxT("Unknown"), 0, wxT("/home/azrael/Music/My Darling.mp3"));
        s1->setID(0);
        s2->setID(1);
        s3->setID(2);
        p->addToPlaylist(s1);
        p->addToPlaylist(s2);
        p->addToPlaylist(s3);
        m = new MediaController(p);
    }
    void TearDown() override{
        delete p;
        delete m;
    }
};

TEST_F(ControllerSuite, search){
    m->searchItem(wxT("Title"));
    ASSERT_EQ(p->getSearchTempList(), p->getPlayList());
    m->searchItem(wxT("0"));
    ASSERT_EQ(*p->getSearchTempList().begin(), nullptr);
}

TEST_F(ControllerSuite, addFile){
    wxArrayString strArray;
    strArray.push_back(wxT("path1"));
    EXPECT_EQ(p->getPlayList().size(), 3);
    m->addFile(&strArray);
    EXPECT_EQ(p->getPlayList().size(), 3);
    strArray.push_back(wxT("/home/azrael/Music/Going Crazy.mp3"));
    m->addFile(&strArray);
    EXPECT_EQ(p->getPlayList().size(), 4);
    strArray.clear();
    m->addFile(nullptr);
    EXPECT_EQ(p->getPlayList().size(), 4);

}

TEST_F(ControllerSuite, deleteSong){
    wxArrayString toDelete;
    toDelete.push_back(wxT("not a good path"));
    m->deleteSong(toDelete);
    EXPECT_EQ(p->getPlayList().size(), 3);
    toDelete.clear();
    toDelete.push_back(wxT("/home/azrael/Music/Honeymoon.mp3"));
    m->deleteSong(toDelete);
    EXPECT_EQ(p->getPlayList().size(), 2);
    toDelete.clear();
    toDelete.push_back(wxT("/home/azrael/Music/Whisper.mp3"));
    toDelete.push_back(wxT("/home/azrael/Music/My Darling.mp3"));
    m->deleteSong(toDelete);
    EXPECT_TRUE(p->getPlayList().empty());
}
TEST_F(ControllerSuite, shuffle){
    //This is empty. Shuffle would not even reach the first line of code meaning that
    //it would not set playing to a random song
    Playlist p2;
    MediaController m2(&p2);
    m2.shuffleList();
    EXPECT_EQ(p2.getPlaying(), nullptr);
    //Here is considered not an empty playlist with no previous playing song
    m->shuffleList();
    EXPECT_NE(p->getPlaying(), nullptr);


}
TEST_F(ControllerSuite, prevNextSong){
    p->setPlaying(s3);
    m->prevSong();
    EXPECT_EQ(p->getPlaying(), s2);
    m->nextSong();
    EXPECT_EQ(p->getPlaying(), s3);
    m->nextSong();
    EXPECT_EQ(p->getPlaying(), s3);
    p->setPlaying(s1);
    m->prevSong();
    EXPECT_EQ(p->getPlaying(), s1);
}
TEST_F(ControllerSuite, setLoop){
    p->setPlaying(s3);
    m->setLoop();
    EXPECT_TRUE(p->getPlaying()->isLoop());
    m->setLoop();
    EXPECT_FALSE(p->getPlaying()->isLoop());
}
TEST_F(ControllerSuite, save){
    EXPECT_TRUE(m->save());
    Playlist p2;
    MediaController m2(&p2);
    //if the list is empty the program doesn't save it
    EXPECT_FALSE(m2.save()); 
}
TEST_F(ControllerSuite, load){
    m->save();
    //load will return always true, even if we don't save the playlist
    //(unless the savedSession file doesn't exist)
    //in that case it returns false
    EXPECT_TRUE(m->load());

}
TEST_F(ControllerSuite, getSongFromPlaylist){
    EXPECT_EQ(s1, m->getSongFromPlaylist(wxT("/home/azrael/Music/Honeymoon.mp3")));
    EXPECT_EQ(nullptr, m->getSongFromPlaylist(wxT("not valid")));
    EXPECT_EQ(nullptr, m->getSongFromPlaylist(wxEmptyString));

}
TEST_F(ControllerSuite, tellPlaylist){
    EXPECT_EQ(nullptr, p->getPlaying());
    m->tellPlaylist(wxT("/home/azrael/Music/Honeymoon.mp3"));
    EXPECT_EQ(m->getSongFromPlaylist(wxT("/home/azrael/Music/Honeymoon.mp3")), p->getPlaying());
    m->tellPlaylist(wxT("not valid"));
    //because this was the last playing song. Since the path is invalid the prev playing doesn't change
    EXPECT_EQ(m->getSongFromPlaylist(wxT("/home/azrael/Music/Honeymoon.mp3")), p->getPlaying());

}
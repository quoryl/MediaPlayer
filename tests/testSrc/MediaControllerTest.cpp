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
        s1 = new Song(wxT("Title1"), wxT("Unknown"), wxT("Unknown"), 0, wxT("/home/azrael/Music/Honeymoon.mp3"));
        s2 = new Song(wxT("Title2"), wxT("Unknown"), wxT("Unknown"), 0, wxT("/home/azrael/Music/Whisper.mp3"));
        s3 = new Song(wxT("Title3"), wxT("Unknown"), wxT("Unknown"), 0, wxT("/home/azrael/Music/My Darling.mp3"));
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

}

TEST_F(ControllerSuite, deleteSong){
    m->deleteSong(wxT("not a good path"));
    EXPECT_EQ(p->getPlayList().size(), 3);
    m->deleteSong(wxT("/home/azrael/Music/Honeymoon.mp3"));
    EXPECT_EQ(p->getPlayList().size(), 2);
}
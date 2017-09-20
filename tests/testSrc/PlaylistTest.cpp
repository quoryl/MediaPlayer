//
// Created by azrael on 10/09/17.
//
#include "../../Model/Playlist.h"
#include "gtest/gtest.h"

class PlaylistSuite : public ::testing::Test {
public:
    Playlist* p;
    Song* s1;
    Song* s2;
    Song* s3;

    void SetUp()override{
        p = new Playlist;

        s1 = new Song(wxT("Title1"));
        s1->setID(0);
        p->addToPlaylist(s1);

        s2 = new Song(wxT("Title2"));
        s2->setID(1);
        p->addToPlaylist(s2);

        s3 = new Song(wxT("Title3"));
        s3->setID(2);
        p->addToPlaylist(s3);

    }
    void TearDown() override{
        delete p;
    }
};

TEST_F(PlaylistSuite, addDelete){
    Song* s = new Song(wxT("titlu"));
    ASSERT_TRUE(p->addToPlaylist(s));
    ASSERT_EQ(4, p->getPlayList().size());
    ASSERT_TRUE(p->deleteFromPlaylist(s));
    ASSERT_EQ(3, p->getPlayList().size());
}

TEST_F(PlaylistSuite, search){
    p->searchPlaylist(wxT("Title1"));
    ASSERT_EQ(*p->getSearchTempList().begin(), s1 );
    p->searchPlaylist(wxT("b"));
    ASSERT_EQ(*p->getSearchTempList().begin(), nullptr);
    p->searchPlaylist(wxT("e3"));
    ASSERT_EQ(*p->getSearchTempList().begin(), s3);
    p->searchPlaylist(wxT("i"));
    ASSERT_EQ(p->getSearchTempList(), p->getPlayList());

}

TEST_F(PlaylistSuite, nowPlaying){
    p->nowPlaying(s1);
    ASSERT_EQ(wxMEDIASTATE_PLAYING, s1->getSongState());
    ASSERT_EQ(s1, p->getPlaying());
    p->nowPlaying(s3);
    ASSERT_EQ(wxMEDIASTATE_PLAYING, s3->getSongState());
    ASSERT_EQ(s3, p->getPlaying());
    ASSERT_EQ(wxMEDIASTATE_STOPPED, s1->getSongState());

}

TEST_F(PlaylistSuite, songChanged){
    vector<long> indexes;
    for(long i = 0; i < 3; i++ )
        indexes.push_back(i);

    p->songChanged(&indexes);
    ASSERT_EQ(wxMEDIASTATE_PLAYING, s1->getSongState());
    ASSERT_EQ(s1, p->getPlaying());

    indexes.clear();

    for(long i = -5; i < -2; i++ )
        indexes.push_back(i);

    //if the index is negative we have a nullptr song meaning that
    //the currently playing song doesn't change
    p->songChanged(&indexes);
    ASSERT_EQ(s1, p->getPlaying());
    indexes.clear();

}

TEST_F(PlaylistSuite, getSong){
    ASSERT_EQ(s1, p->getSong(0));
    ASSERT_EQ(s2, p->getSong(1));
    ASSERT_EQ(s3, p->getSong(2));
    ASSERT_EQ(nullptr, p->getSong(-6));
    ASSERT_EQ(nullptr, p->getSong(100025));
    ASSERT_EQ(nullptr, p->getSong(-99999));
}
TEST_F(PlaylistSuite, getSetPlaying){
    EXPECT_EQ(nullptr, p->getPlaying());
    p->setPlaying(s2) ;
    EXPECT_EQ(s2, p->getPlaying());
    p->setPlaying(s3) ;
    EXPECT_EQ(s3, p->getPlaying());
    p->setPlaying(nullptr) ;
    EXPECT_EQ(s3, p->getPlaying());
}
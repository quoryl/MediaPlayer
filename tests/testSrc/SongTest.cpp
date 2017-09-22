//
// Created by azrael on 09/09/17.
//

#include "../../Model/Song.h"
#include "gtest/gtest.h"

class SongSuite : public ::testing::Test {
public:
    Song* s1 ;
    Song* s2 ;
    Song* s3 ;

    void SetUp()override{
        wxBitmap noArt;
        noArt.LoadFile(wxT("/home/azrael/CLionProjects/MediaPlayer/ControlsPNG/NoAlbumArt.png"));
        s1 = new Song(wxT("someTitle"), noArt);
        s2 = new Song(wxT("someOtherTitle"),noArt, wxT("2ne1"), wxT("Crush"), 306 , wxT("/home/azrael/Music/Whisper.mp3"), wxT("dance"));
        s3 = new Song(wxT("creativeTitle"), noArt, wxT("2ne1"), wxT("Crush"), -5 , wxT("/home/azrael/Music/Whisper.mp3"), wxT("dance"));
    }
    void TearDown() override{
        delete s1;
        delete s2;
        delete s3;
    }
};

TEST_F(SongSuite, Constructor){
//here are also checked setLength, setLoop, setSongPath,
    ASSERT_STREQ(wxT("someTitle"), s1->getTitle());
    ASSERT_STREQ(wxEmptyString, s1->getSongPath());
    ASSERT_STREQ(wxT("Unknown"), s1->getAlbum());
    ASSERT_STREQ(wxT("Unknown"), s1->getArtist());
    ASSERT_STREQ(wxT("Unknown"), s1->getGenre());
    ASSERT_EQ(0, s1->getLength());
    ASSERT_EQ(-1, s1->getID());
    EXPECT_FALSE(s1->isLoop());
    ASSERT_EQ(wxMEDIASTATE_STOPPED, s1->getSongState());


    ASSERT_STREQ(wxT("someOtherTitle"), s2->getTitle());
    ASSERT_STREQ(wxT("/home/azrael/Music/Whisper.mp3"), s2->getSongPath());
    ASSERT_STREQ(wxT("Crush"), s2->getAlbum());
    ASSERT_STREQ(wxT("2ne1"), s2->getArtist());
    ASSERT_STREQ(wxT("dance"), s2->getGenre());
    ASSERT_EQ(306, s2->getLength());
    ASSERT_EQ(-1, s2->getID());
    EXPECT_FALSE(s2->isLoop());

    ASSERT_STREQ(wxT("creativeTitle"), s3->getTitle());
    ASSERT_STREQ(wxT("/home/azrael/Music/Whisper.mp3"), s3->getSongPath());
    ASSERT_STREQ(wxT("dance"), s2->getGenre());
    ASSERT_EQ(0, s3->getLength());
    ASSERT_EQ(-1, s3->getID());
    EXPECT_FALSE(s3->isLoop());


}

TEST_F( SongSuite, positiveIDs){
    s1->setID(-10);
    ASSERT_EQ(-1, s1->getID());
    s1->setID(100000000000);
    ASSERT_EQ(100000000000, s1->getID());
    s1->setID(0);
    ASSERT_EQ(0, s1->getID());
}

TEST_F(SongSuite, checkMediaState){
    ASSERT_EQ(wxMEDIASTATE_STOPPED, s1->getSongState() );
    s1->setSongState(wxMEDIASTATE_PAUSED);
    ASSERT_EQ(wxMEDIASTATE_PAUSED, s1->getSongState() );
    s1->setSongState(wxMEDIASTATE_PLAYING);
    ASSERT_EQ(wxMEDIASTATE_PLAYING, s1->getSongState() );

}
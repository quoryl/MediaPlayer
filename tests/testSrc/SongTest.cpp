//
// Created by azrael on 09/09/17.
//

#include "../../Model/Song.h"
#include "gtest/gtest.h"

TEST(Song, Constructor){
    Song s1(wxT("4"));
    ASSERT_STREQ(wxT("4"), s1.getTitle());
    ASSERT_STREQ(wxEmptyString, s1.getSongPath());
    ASSERT_STREQ(wxT("Unknown"), s1.getAlbum());
    ASSERT_STREQ(wxT("Unknown"), s1.getArtist());
    ASSERT_EQ(0, s1.getLength());
    ASSERT_EQ(-1, s1.getID());
    EXPECT_FALSE(s1.isLoop());

    Song s2(wxT("someOtherTitle"), wxT("2ne1"), wxT("Crush"), 306 , wxT("/home/azrael/Music/Whisper.mp3"));
    ASSERT_STREQ(wxT("someOtherTitle"), s2.getTitle());
    ASSERT_STREQ(wxT("/home/azrael/Music/Whisper.mp3"), s2.getSongPath());

    ASSERT_EQ(306, s2.getLength());
    ASSERT_EQ(-1, s2.getID());
    EXPECT_FALSE(s2.isLoop());

    Song* s3 = new Song(wxT("creativeTitle"), wxT("2ne1"), wxT("Crush"), -5 , wxT("/home/azrael/Music/Whisper.mp3"));
    ASSERT_STREQ(wxT("creativeTitle"), s3->getTitle());
    ASSERT_STREQ(wxT("/home/azrael/Music/Whisper.mp3"), s3->getSongPath());
    ASSERT_EQ(0, s3->getLength());
    ASSERT_EQ(-1, s3->getID());
    EXPECT_FALSE(s3->isLoop());

    delete s3;

}

TEST( Song, positiveIDs){
    Song s1(wxT("creativeIndeed"));
    s1.setID(-10);
    ASSERT_EQ(-1, s1.getID());
    s1.setID(1000);
    ASSERT_EQ(1000, s1.getID());
    s1.setID(0);
    ASSERT_EQ(0, s1.getID());
}

TEST(Song, checkMediaState){
    Song s1(wxT("not creative"));
    ASSERT_EQ(wxMEDIASTATE_STOPPED, s1.getSongState() );
    s1.setSongState(wxMEDIASTATE_PAUSED);
    ASSERT_EQ(wxMEDIASTATE_PAUSED, s1.getSongState() );
    s1.setSongState(wxMEDIASTATE_PLAYING);
    ASSERT_EQ(wxMEDIASTATE_PLAYING, s1.getSongState() );

}
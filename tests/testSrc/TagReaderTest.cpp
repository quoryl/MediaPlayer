//
// Created by azrael on 11/09/17.
//
#include <gtest/gtest.h>
#include "../../Controller/TagReader.h"
class TagReaderSuite : public ::testing::Test{
public:
    TagReader* tags;
    TagReader* tags2;
    TagReader* tags3;
    void SetUp()override {
        wxString path = wxT("/home/azrael/Music/DNA.mp3");
        auto charPath = static_cast<const char *>(path);
        tags = new TagReader(charPath);

        wxString path2 = wxT("poof.mp3");
        auto charPath2 = static_cast<const char*>(path2);
        tags2 = new TagReader(charPath2);

        wxString path3 = wxEmptyString;
        auto charPath3 = static_cast<const char*>(path3);
        tags3 = new TagReader(charPath3);
    }
    bool compareImages(wxImage a, wxImage b){
        if(a.GetSize() == b.GetSize()) {
            for (int i = 0; i < a.GetWidth(); i++) {
                for (int j = 0; j < a.GetHeight(); j++) {
                    if(a.GetGreen(i, j) != b.GetGreen(i, j) || a.GetRed(i, j) != b.GetRed(i, j) || a.GetBlue(i, j) != b.GetBlue(i, j)){
                        return false;
                    }
                }
            }
            return true;
        }
        return false;
    }
    void TearDown() override {
        delete tags;
        delete tags2;
        delete tags3;
    }
};
TEST_F(TagReaderSuite, ConstructorGetTags){


    EXPECT_STREQ(wxT("DNA"), tags->getTitle());
    EXPECT_STREQ(wxT("BTS"), tags->getArtist());
    EXPECT_STREQ(wxT("LOVE YOURSELF : Her"), tags->getAlbum());
    EXPECT_STREQ(wxT("Rap / Hip-hop"), tags->getGenre());

    EXPECT_STREQ(wxEmptyString, tags2->getTitle());
    EXPECT_STREQ(wxEmptyString, tags2->getArtist());
    EXPECT_STREQ(wxEmptyString, tags2->getAlbum());
    EXPECT_STREQ(wxEmptyString, tags2->getGenre());

    EXPECT_STREQ(wxEmptyString, tags3->getTitle());
    EXPECT_STREQ(wxEmptyString, tags3->getArtist());
    EXPECT_STREQ(wxEmptyString, tags3->getAlbum());
    EXPECT_STREQ(wxEmptyString, tags3->getGenre());

    TagReader tags4(nullptr);
    EXPECT_STREQ(wxEmptyString, tags4.getTitle());
    EXPECT_STREQ(wxEmptyString, tags4.getArtist());
    EXPECT_STREQ(wxEmptyString, tags4.getAlbum());
    EXPECT_STREQ(wxEmptyString, tags4.getGenre());
}
TEST_F(TagReaderSuite, processAlbumArt){

    EXPECT_TRUE(tags->processAlbumArt());

    EXPECT_FALSE(tags2->processAlbumArt());

    EXPECT_FALSE(tags3->processAlbumArt());

    TagReader tags4(nullptr);
    EXPECT_FALSE(tags4.processAlbumArt());

    wxString path5 = wxT("/home/azrael/Music/DINOSAUR.wav");
    auto charPath5 = static_cast<const char*>(path5);
    TagReader tags5(charPath5);
    EXPECT_FALSE(tags5.processAlbumArt());
}
TEST_F(TagReaderSuite, getAlbumArt){
    wxImage DNAImage = tags->getAlbumArt().ConvertToImage();

    wxString BOMpath = wxT("/home/azrael/Music/Best Of Me.mp3");
    auto charBOMPath = static_cast<const char *>(BOMpath);
    TagReader BOMtags(charBOMPath);
    wxImage BOMImage = BOMtags.getAlbumArt().ConvertToImage();

    EXPECT_TRUE(compareImages(BOMImage, DNAImage));

    wxString TodayPath = wxT("/home/azrael/Music/Today.mp3");
    auto charTodayPath = static_cast<const char *>(TodayPath);
    TagReader TodayTags(charTodayPath);
    wxImage TodayImage = TodayTags.getAlbumArt().ConvertToImage();

    EXPECT_FALSE(compareImages(DNAImage, TodayImage));

    wxString DinoPath = wxT("/home/azrael/Music/DINOSAUR.wav");
    auto charDinoPath = static_cast<const char *>(DinoPath);
    TagReader dinoTags(charDinoPath);
    wxImage dinoImage = dinoTags.getAlbumArt().ConvertToImage();

    wxBitmap noArtB;
    noArtB.LoadFile(wxT("/home/azrael/CLionProjects/MediaPlayer/ControlsPNG/NoAlbumArt.png"));
    wxImage noArtImg = noArtB.ConvertToImage();

    EXPECT_TRUE(compareImages(noArtImg, dinoImage));

    wxImage emptyPathImage = tags3->getAlbumArt().ConvertToImage();

    EXPECT_TRUE(compareImages(noArtImg, emptyPathImage));

    TagReader nullPath(nullptr);
    wxImage nullPathImage = nullPath.getAlbumArt().ConvertToImage();

    EXPECT_TRUE(compareImages(noArtImg, emptyPathImage));


}
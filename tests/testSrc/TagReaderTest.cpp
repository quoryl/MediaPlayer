//
// Created by azrael on 11/09/17.
//
#include <gtest/gtest.h>
#include "../../Controller/TagReader.h"
TEST(TagReader, ConstructorGetTitle){

    wxString path = wxT("/home/azrael/Music/DNA.mp3");
    auto charPath = static_cast<const char*>(path);
    TagReader tags(charPath);

    EXPECT_STREQ(wxT("DNA"), tags.getTitle());

    wxString path2 = wxT("poof.mp3");
    auto charPath2 = static_cast<const char*>(path2);
    TagReader tags2(charPath2);

    EXPECT_STREQ(wxEmptyString, tags2.getTitle());

    wxString path3 = wxEmptyString;
    auto charPath3 = static_cast<const char*>(path3);
    TagReader tags3(charPath3);

    EXPECT_STREQ(wxEmptyString, tags3.getTitle());

    TagReader tags4(nullptr);
    EXPECT_STREQ(wxEmptyString, tags4.getTitle());
}
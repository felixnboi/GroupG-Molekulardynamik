#include <gtest/gtest.h>
#include <array>
#include <string>

#include "../src/data/Cuboid.h"
#include "../src/data/Disc.h"
#include "../src/data/SimData.h"
#include "../src/io/input/XMLReader.h"

TEST(XMLReader, parseXMLFile){
    XMLReader xmlreader;

    std::vector<Cuboid> cuboids;
    std::vector<Disc> discs;

    Cuboid cuboid({12,13,14},{15,16,17},{18,19,20},21,22,7);
    Disc disc({23,24,25},{26,27,28},29,30,31);

    SimData simdata;

    xmlreader.readCuboids("../input/test.xml", cuboids);
    xmlreader.readDiscs("../input/test.xml", discs);

    xmlreader.readSimulation("../input/test.xml", simdata);

    for(const auto& c : cuboids){
        EXPECT_TRUE(c == cuboid);
    }

    for(const auto& d : discs){
        EXPECT_TRUE(d == disc);
    }

    EXPECT_TRUE(simdata.getInputFile()=="test1");
    EXPECT_TRUE(simdata.getBaseName()=="test2");
    EXPECT_TRUE(simdata.getWriteFrequency()==1);
    EXPECT_TRUE(simdata.getStartTime()==4);
    EXPECT_TRUE(simdata.getEndTime()==2);
    EXPECT_TRUE(simdata.getDeltaT()==3);
    EXPECT_TRUE(simdata.getForceStr()=="lennardJonesForce");
    EXPECT_TRUE(simdata.getAlgorithm()=="linkedcell");
    EXPECT_TRUE(simdata.getLoglevel()=="INFO");
    EXPECT_TRUE((simdata.getBoundary()==std::array<std::string, 6>{"outflow", "mirror", "reflecting", "outflow", "mirror", "reflecting"}));
    EXPECT_TRUE(simdata.getCutoffRadius()==11);
    EXPECT_TRUE((simdata.getDomain()==std::array<double, 3>{8, 9, 10}));
    EXPECT_TRUE(simdata.getSigma()==6);
    EXPECT_TRUE(simdata.getEpsilon()==5);
}
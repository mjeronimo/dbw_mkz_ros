/*********************************************************************
 * C++ unit test for dbw_mkz_can/PlatformMap.h
 *********************************************************************/

#include <gtest/gtest.h>

// File under test
#include <dbw_mkz_can/PlatformMap.h>
using namespace dbw_mkz_can;

// Test constructors
TEST(PlatformMap, constructor)
{
  const Platform p = (Platform)1;
  const Module m = (Module)2;

  // Empty
  EXPECT_EQ(ModuleVersion(), PlatformMap().findModule(p,m));

  // Individual fields
  EXPECT_EQ(ModuleVersion(1,2,3), PlatformMap(p,m,ModuleVersion(1,2,3)).findModule(p,m));

  // PlatformVersion class
  EXPECT_EQ(ModuleVersion(1,2,3), PlatformMap(PlatformVersion(p,m, ModuleVersion(1,2,3))).findModule(p,m));

  // Vector of PlatformVersion
  EXPECT_EQ(ModuleVersion(1,2,3), PlatformMap({PlatformVersion(p,m, ModuleVersion(1,2,3))}).findModule(p,m));
}

// Test the find method
TEST(PlatformMap, findModule)
{
  const Platform px = (Platform)0; const Module mx = (Module)0;
  const Platform py = (Platform)1; const Module my = (Module)1;
  const Platform pz = (Platform)2; const Module mz = (Module)2;
  const Platform pw = (Platform)3; const Module mw = (Module)3;

  // Construct PlatformMap with multiple platforms and modules
  const PlatformMap x({
    {PlatformVersion(px, mx, ModuleVersion(10,11,12))},
    {PlatformVersion(px, my, ModuleVersion(20,21,22))},
    {PlatformVersion(px, mz, ModuleVersion(30,31,32))},
    {PlatformVersion(py, mx, ModuleVersion(40,41,42))},
    {PlatformVersion(py, my, ModuleVersion(50,51,52))},
    {PlatformVersion(py, mz, ModuleVersion(60,61,62))},
    {PlatformVersion(pz, mx, ModuleVersion(70,71,72))},
    {PlatformVersion(pz, my, ModuleVersion(80,81,82))},
    {PlatformVersion(pz, mz, ModuleVersion(90,91,92))},
  });

  // Find each entry
  EXPECT_EQ(ModuleVersion(10,11,12), x.findModule(px,mx));
  EXPECT_EQ(ModuleVersion(20,21,22), x.findModule(px,my));
  EXPECT_EQ(ModuleVersion(30,31,32), x.findModule(px,mz));
  EXPECT_EQ(ModuleVersion(40,41,42), x.findModule(py,mx));
  EXPECT_EQ(ModuleVersion(50,51,52), x.findModule(py,my));
  EXPECT_EQ(ModuleVersion(60,61,62), x.findModule(py,mz));
  EXPECT_EQ(ModuleVersion(70,71,72), x.findModule(pz,mx));
  EXPECT_EQ(ModuleVersion(80,81,82), x.findModule(pz,my));
  EXPECT_EQ(ModuleVersion(90,91,92), x.findModule(pz,mz));

  // Find entries that don't exist
  EXPECT_EQ(ModuleVersion(), x.findModule(px, mw));
  EXPECT_EQ(ModuleVersion(), x.findModule(pw, mx));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


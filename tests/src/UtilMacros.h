#ifndef UTILMACROS_H
#define UTILMACROS_H

#define EXPECT_VEC3_EQ(A, B) \
  EXPECT_FLOAT_EQ((A).x, (B).x); \
  EXPECT_FLOAT_EQ((A).y, (B).y); \
  EXPECT_FLOAT_EQ((A).z, (B).z);

#define EXPECT_VEC3_NE(A, B) \
  EXPECT_NE((A).x, (B).x); \
  EXPECT_NE((A).y, (B).y); \
  EXPECT_NE((A).z, (B).z);

#endif // UTILMACROS_H

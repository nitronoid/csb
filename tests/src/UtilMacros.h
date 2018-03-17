#ifndef UTILMACROS_H
#define UTILMACROS_H

#define EXPECT_VEC3_EQ(A, B) \
  EXPECT_FLOAT_EQ((A).x, (B).x); \
  EXPECT_FLOAT_EQ((A).y, (B).y); \
  EXPECT_FLOAT_EQ((A).z, (B).z);

#endif // UTILMACROS_H

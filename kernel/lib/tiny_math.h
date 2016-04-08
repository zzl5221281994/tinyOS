#ifndef tiny_math_H
#define tiny_math_H
#define           min(         X      ,Y          )                      \
                                        ({                     \
                                           typeof (X) x_ = (X);\  
                                           typeof (Y) y_ = (Y);\  
                                           (x_ < y_) ? x_ : y_;\
			                            })
#define           max(         X      ,Y          )                      \
                                        ({                     \
                                           typeof (X) x_ = (X);\  
                                           typeof (Y) y_ = (Y);\  
                                           (x_ > y_) ? x_ : y_;\
			                            })
#endif
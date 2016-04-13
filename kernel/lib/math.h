/************************************************************************************
Wed Apr 13 15:37:28 2016

MIT License
Copyright (c) 2016 zhuzuolang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************************************/
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
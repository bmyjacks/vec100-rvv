/****************************************************************************
 *
 *
 *  Project: BLIS 2.1
 *  Source files:
 *    frame/include/bli_lang_defs.h
 *    frame/include/bli_macro_defs.h
 *    frame/include/bli_kernel_macro_defs.h
 *    frame/include/bli_pragma_macro_defs.h
 *    frame/include/bli_param_macro_defs.h
 *    frame/include/bli_cast_macro_defs.h
 *    frame/include/bli_complex_macro_defs.h
 *    frame/include/level0/bli_assigns.h
 *    frame/include/level0/bli_declinits.h
 *    frame/include/level0/bli_complex_terms.h
 *    frame/include/bli_misc_macro_defs.h
 *    frame/include/level0/bli_tscal2s.h
 *    frame/include/level0/bli_tcopys.h
 *    frame/include/level0/bli_tsets.h
 *    ref_kernels/1m/bli_packm_cxk_1er_ref.c
 *
 *
 *  The original file copyright and license notices follow.
 *
 *
 * frame/include/bli_lang_defs.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *    Copyright (C) 2018 - 2019, Advanced Micro Devices, Inc.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/bli_macro_defs.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *    Copyright (C) 2018 - 2019, Advanced Micro Devices, Inc.
 *    Copyright (C) 2024, Southern Methodist University
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/bli_kernel_macro_defs.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/bli_pragma_macro_defs.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2019, The University of Texas at Austin
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/bli_param_macro_defs.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *    Copyright (C) 2018 - 2019, Advanced Micro Devices, Inc.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/bli_cast_macro_defs.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *    Copyright (C) 2024, Southern Methodist University
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/bli_complex_macro_defs.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *    Copyright (C) 2023, Southern Methodist University
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/level0/bli_assigns.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *    Copyright (C) 2024, Southern Methodist University
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/level0/bli_declinits.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *    Copyright (C) 2024, Southern Methodist University
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/level0/bli_complex_terms.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *    Copyright (C) 2024, Southern Methodist University
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/bli_misc_macro_defs.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/level0/bli_tscal2s.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *    Copyright (C) 2024, Southern Methodist University
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/level0/bli_tcopys.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *    Copyright (C) 2024, Southern Methodist University
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * frame/include/level0/bli_tsets.h
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2014, The University of Texas at Austin
 *    Copyright (C) 2024, Southern Methodist University
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ref_kernels/1m/bli_packm_cxk_1er_ref.c
 *
 *    BLIS
 *    An object-based framework for developing high-performance BLAS-like
 *    libraries.
 *
 *    Copyright (C) 2024, Southern Methodist University
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are
 *    met:
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     - Neither the name(s) of the copyright holder(s) nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "kernel.h"

/*
 * frame/include/bli_lang_defs.h:42-46
 */
#define restrict

/*
 * frame/include/bli_macro_defs.h:50-51
 */
#define PASTEMAC1_(ch, op) bli_##ch##op
#define PASTEMAC2_(ch1, ch2, op) bli_##ch1##ch2##op

/*
 * frame/include/bli_macro_defs.h:53
 */
#define PASTEMAC4_(ch1, ch2, ch3, ch4, op) bli_##ch1##ch2##ch3##ch4##op

/*
 * frame/include/bli_macro_defs.h:57-59
 */
#define PASTEMAC__(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ...)        \
    PASTEMAC##arg8##_
#define PASTEMAC_(...) PASTEMAC__(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0, XXX)
#define PASTEMAC(...) PASTEMAC_(__VA_ARGS__)(__VA_ARGS__)

/*
 * frame/include/bli_macro_defs.h:62
 */
#define PASTECH1_(ch, op) ch##op

/*
 * frame/include/bli_macro_defs.h:69-71
 */
#define PASTECH__(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ...)         \
    PASTECH##arg8##_
#define PASTECH_(...) PASTECH__(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0, XXX)
#define PASTECH(...) PASTECH_(__VA_ARGS__)(__VA_ARGS__)

/*
 * frame/include/bli_kernel_macro_defs.h:263-264
 */
#define BLIS_MR_d 4

/*
 * frame/include/bli_kernel_macro_defs.h:279-280
 */
#define BLIS_NR_d 8

/*
 * frame/include/bli_kernel_macro_defs.h:295-296
 */
#define BLIS_BBM_d 1

/*
 * frame/include/bli_kernel_macro_defs.h:311-312
 */
#define BLIS_BBN_d 1

/*
 * frame/include/bli_pragma_macro_defs.h:47-48
 */
#define PRAGMA_OMP_SIMD _Pragma("omp simd")

/*
 * frame/include/bli_pragma_macro_defs.h:64-67
 */
#define PRAGMA_SIMD PRAGMA_OMP_SIMD

/*
 * frame/include/bli_param_macro_defs.h:391-395
 */
inline bool bli_is_conj(conj_t conj) { return (bool)(conj == BLIS_CONJUGATE); }

/*
 * frame/include/bli_param_macro_defs.h:1104-1108
 */
inline bool bli_is_1e_packed(pack_t schema) {
    return (bool)((schema & BLIS_PACK_FORMAT_BITS) == BLIS_BITVAL_1E);
}

/*
 * frame/include/bli_cast_macro_defs.h:138-141
 */
inline double bli_ddcast(double d) { return d; }

/*
 * frame/include/bli_cast_macro_defs.h:245
 */
#define bli_ddtcast bli_ddcast

/*
 * frame/include/bli_cast_macro_defs.h:315
 */
#define bli_dzero 0.0

/*
 * frame/include/bli_cast_macro_defs.h:351-355
 */
#define bli_dmul(a, b) (a) * (b)
#define bli_dadd(a, b) (a) + (b)
#define bli_dsub(a, b) (a) - (b)
#define bli_dneg(a) -(a)

/*
 * frame/include/bli_complex_macro_defs.h:45-46
 */
#define bli_dreal(x) (x)
#define bli_dimag(x) (0.0)

/*
 * frame/include/level0/bli_assigns.h:47-48
 */
#define bli_rassigns(xr, xi, yr, yi)                                           \
    {                                                                          \
        yr = xr;                                                               \
        (void)(xi);                                                            \
        (void)(yi);                                                            \
    }
#define bli_cassigns(xr, xi, yr, yi)                                           \
    {                                                                          \
        yr = xr;                                                               \
        yi = xi;                                                               \
    }

/*
 * frame/include/level0/bli_declinits.h:51-52
 */
#define bli_cdeclinits(pxy, xr, xi, yr, yi)                                    \
    PASTEMAC(pxy, ctype) yr = xr;                                              \
    (void)yr;                                                                  \
    PASTEMAC(pxy, ctype) yi = xi;                                              \
    (void)yi;

/*
 * frame/include/level0/bli_complex_terms.h:51
 */
#define bli_cctermrr(pab, ab) ab

/*
 * frame/include/level0/bli_complex_terms.h:57
 */
#define bli_cctermii(pab, ab) ab

/*
 * frame/include/level0/bli_complex_terms.h:63
 */
#define bli_cctermir(pab, ab) ab

/*
 * frame/include/level0/bli_complex_terms.h:69
 */
#define bli_cctermri(pab, ab) ab

/*
 * frame/include/bli_misc_macro_defs.h:135
 */
#define bli_dctype double

/*
 * frame/include/bli_misc_macro_defs.h:156
 */
#define bli_dprec d

/*
 * frame/include/bli_misc_macro_defs.h:158
 */
#define bli_zprec d

/*
 * frame/include/bli_misc_macro_defs.h:164
 */
#define bli_ddom r

/*
 * frame/include/bli_misc_macro_defs.h:166
 */
#define bli_zdom c

/*
 * frame/include/level0/bli_tscal2s.h:46-103
 */
#define bli_tscal2ims(                                                         \
                                                                               \
    da, pa, ar, ai, dx, px, xr, xi, dy, py, yr, yi, chc)                       \
    {                                                                          \
        PASTEMAC(c, declinits)                                                 \
        (py,                                                                   \
         PASTEMAC(chc, py, tcast)(PASTEMAC(chc, sub)(                          \
             PASTEMAC(da, dx, termrr)(                                         \
                 chc, PASTEMAC(chc, mul)(PASTEMAC(pa, chc, tcast)(ar),         \
                                         PASTEMAC(px, chc, tcast)(xr))),       \
             PASTEMAC(da, dx, termii)(                                         \
                 chc, PASTEMAC(chc, mul)(PASTEMAC(pa, chc, tcast)(ai),         \
                                         PASTEMAC(px, chc, tcast)(xi))))),     \
         PASTEMAC(chc, py, tcast)(PASTEMAC(chc, add)(                          \
             PASTEMAC(da, dx, termir)(                                         \
                 chc, PASTEMAC(chc, mul)(PASTEMAC(pa, chc, tcast)(ai),         \
                                         PASTEMAC(px, chc, tcast)(xr))),       \
             PASTEMAC(da, dx, termri)(                                         \
                 chc, PASTEMAC(chc, mul)(PASTEMAC(pa, chc, tcast)(ar),         \
                                         PASTEMAC(px, chc, tcast)(xi))))),     \
         tr, ti);                                                              \
        PASTEMAC(dy, assigns)                                                  \
        (tr, ti, yr, yi);                                                      \
    }

/*
 * frame/include/level0/bli_tscal2s.h:152-168
 */
#define bli_tscal2ris(cha, chx, chy, chc, ar, ai, xr, xi, yr, yi)              \
    bli_tscal2ims(PASTEMAC(cha, dom), PASTEMAC(cha, prec), ar, ai,             \
                  PASTEMAC(chx, dom), PASTEMAC(chx, prec), xr, xi,             \
                  PASTEMAC(chy, dom), PASTEMAC(chy, prec), yr, yi,             \
                  PASTEMAC(chc, prec))

/*
 * frame/include/level0/bli_tscal2s.h:171-188
 */
#define bli_tscal2jris(cha, chx, chy, chc, ar, ai, xr, xi, yr, yi)             \
    bli_tscal2ims(PASTEMAC(cha, dom), PASTEMAC(cha, prec), ar, ai,             \
                  PASTEMAC(chx, dom), PASTEMAC(chx, prec), xr,                 \
                  PASTEMAC(PASTEMAC(chx, prec), neg)(xi), PASTEMAC(chy, dom),  \
                  PASTEMAC(chy, prec), yr, yi, PASTEMAC(chc, prec))

/*
 * frame/include/level0/bli_tcopys.h:44-57
 */
#define bli_tcopyims(                                                          \
                                                                               \
    dx, px, xr, xi, dy, py, yr, yi)                                            \
    {                                                                          \
        PASTEMAC(dy, assigns)                                                  \
        (PASTEMAC(px, py, tcast)(xr), PASTEMAC(px, py, tcast)(xi), yr, yi);    \
    }

/*
 * frame/include/level0/bli_tcopys.h:96-107
 */
#define bli_tcopyris(chx, chy, xr, xi, yr, yi)                                 \
    bli_tcopyims(PASTEMAC(chx, dom), PASTEMAC(chx, prec), xr, xi,              \
                 PASTEMAC(chy, dom), PASTEMAC(chy, prec), yr, yi)

/*
 * frame/include/level0/bli_tsets.h:41-53
 */
#define bli_tsetims(dx, px, xr, xi, dy, py, yr, yi)                            \
    {                                                                          \
        PASTEMAC(dy, assigns)                                                  \
        (PASTEMAC(px, py, tcast)(xr), PASTEMAC(px, py, tcast)(xi), yr, yi);    \
    }

/*
 * frame/include/level0/bli_tsets.h:88-99
 */
#define bli_tsets(chx, chy, xr, xi, y)                                         \
    bli_tsetims(PASTEMAC(chx, dom), PASTEMAC(chx, prec), xr, xi,               \
                PASTEMAC(chy, dom), PASTEMAC(chy, prec),                       \
                PASTEMAC(chy, real)(y), PASTEMAC(chy, imag)(y))

/*
 * frame/include/level0/bli_tsets.h:144-159
 */
#define bli_tset0s(chy, y)                                                     \
    bli_tsets(chy, chy, PASTEMAC(PASTEMAC(chy, prec), zero),                   \
              PASTEMAC(PASTEMAC(chy, prec), zero), y)

/*
 * frame/include/level0/bli_tsets.h:209-214
 */
#define bli_tset0s_mxn(chy, m, n, y, rs_y, cs_y)                               \
    {                                                                          \
        for (dim_t _j = 0; _j < (n); ++_j)                                     \
            for (dim_t _i = 0; _i < (m); ++_i)                                 \
                bli_tset0s(chy, *((y) + _i * (rs_y) + _j * (cs_y)));           \
    }

/*
 * frame/include/level0/bli_tsets.h:290-313
 */
#define bli_tset0s_edge(chp, i, m, j, n, p, ldp)                               \
    {                                                                          \
        if ((i) < (m)) {                                                       \
            bli_tset0s_mxn(chp, (m) - (i), j, (p) + (i) * 1, 1, ldp);          \
        }                                                                      \
                                                                               \
        if ((j) < (n)) {                                                       \
            bli_tset0s_mxn(chp, m, (n) - (j), (p) + (j) * (ldp), 1, ldp);      \
        }                                                                      \
    }

/*
 * ref_kernels/1m/bli_packm_cxk_1er_ref.c:38-63
 */
#define PACKM_1E_BODY(ctypep_r, cha, chp, pragma, cdim, dfac, inca2, op)       \
                                                                               \
    do {                                                                       \
        for (dim_t k = n; k != 0; --k) {                                       \
            pragma for (dim_t mn = 0; mn < cdim; ++mn) {                       \
                ctypep_r ka_r, ka_i;                                           \
                PASTEMAC(t, op)(chp, cha, chp, chp, kappa_r, kappa_i,          \
                                *(alpha1 + mn * inca2 + 0),                    \
                                *(alpha1 + mn * inca2 + 1), ka_r, ka_i);       \
                for (dim_t d = 0; d < dfac; ++d) {                             \
                    bli_tcopyris(chp, chp, ka_r, ka_i,                         \
                                 *(pi1_ri + (mn * 2 + 0) * dfac + d),          \
                                 *(pi1_ri + (mn * 2 + 1) * dfac + d));         \
                    bli_tcopyris(chp, chp, -ka_i, ka_r,                        \
                                 *(pi1_ir + (mn * 2 + 0) * dfac + d),          \
                                 *(pi1_ir + (mn * 2 + 1) * dfac + d));         \
                }                                                              \
            }                                                                  \
                                                                               \
            alpha1 += lda2;                                                    \
            pi1_ri += ldp2;                                                    \
            pi1_ir += ldp2;                                                    \
        }                                                                      \
    } while (0)

/*
 * ref_kernels/1m/bli_packm_cxk_1er_ref.c:66-88
 */
#define PACKM_1R_BODY(ctypep_r, cha, chp, pragma, cdim, dfac, inca2, op)       \
                                                                               \
    do {                                                                       \
        for (dim_t k = n; k != 0; --k) {                                       \
            pragma for (dim_t mn = 0; mn < cdim; ++mn) {                       \
                ctypep_r ka_r, ka_i;                                           \
                PASTEMAC(t, op)(chp, cha, chp, chp, kappa_r, kappa_i,          \
                                *(alpha1 + mn * inca2 + 0),                    \
                                *(alpha1 + mn * inca2 + 1), ka_r, ka_i);       \
                for (dim_t d = 0; d < dfac; ++d)                               \
                    bli_tcopyris(chp, chp, ka_r, ka_i,                         \
                                 *(pi1_r + mn * dfac + d),                     \
                                 *(pi1_i + mn * dfac + d));                    \
            }                                                                  \
                                                                               \
            alpha1 += lda2;                                                    \
            pi1_r += ldp2;                                                     \
            pi1_i += ldp2;                                                     \
        }                                                                      \
    } while (0)

/*
 * ref_kernels/1m/bli_packm_cxk_1er_ref.c:91-220
 */
#define GENTFUNC2R(ctypea, ctypea_r, cha, cha_r, ctypep, ctypep_r, chp, chp_r, \
                   opname, arch, suf)                                          \
                                                                               \
    void PASTEMAC(cha, chp, opname, arch, suf)(                                \
        conj_t conja, pack_t schema, dim_t cdim, dim_t cdim_max,               \
        dim_t cdim_bcast, dim_t n, dim_t n_max, const void *kappa,             \
        const void *a, inc_t inca, inc_t lda, void *p, inc_t ldp,              \
        const void *params, const cntx_t *cntx) {                              \
        const dim_t mr = PASTECH(BLIS_MR_, chp_r);                             \
        const dim_t nr = PASTECH(BLIS_NR_, chp_r);                             \
        const dim_t bbm = PASTECH(BLIS_BBM_, chp_r);                           \
        const dim_t bbn = PASTECH(BLIS_BBN_, chp_r);                           \
                                                                               \
        if (bli_is_1e_packed(schema)) {                                        \
            const dim_t cdim2 = 2 * cdim;                                      \
            const inc_t inca2 = 2 * inca;                                      \
            const inc_t lda2 = 2 * lda;                                        \
            const inc_t ldp2 = 2 * ldp;                                        \
                                                                               \
            ctypep_r kappa_r = ((ctypep_r *)kappa)[0];                         \
            ctypep_r kappa_i = ((ctypep_r *)kappa)[1];                         \
            const ctypea_r *restrict alpha1 = (ctypea_r *)a;                   \
            ctypep_r *restrict pi1_ri = (ctypep_r *)p;                         \
            ctypep_r *restrict pi1_ir = (ctypep_r *)p + ldp;                   \
                                                                               \
            if (cdim2 == mr && cdim_bcast == bbm && mr != -1) {                \
                if (inca == 1) {                                               \
                    if (bli_is_conj(conja))                                    \
                        PACKM_1E_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, mr / 2, \
                                      bbm, 2, scal2jris);                      \
                    else                                                       \
                        PACKM_1E_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, mr / 2, \
                                      bbm, 2, scal2ris);                       \
                } else {                                                       \
                    if (bli_is_conj(conja))                                    \
                        PACKM_1E_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, mr / 2, \
                                      bbm, inca2, scal2jris);                  \
                    else                                                       \
                        PACKM_1E_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, mr / 2, \
                                      bbm, inca2, scal2ris);                   \
                }                                                              \
            } else if (cdim2 == nr && cdim_bcast == bbn && nr != -1) {         \
                if (inca == 1) {                                               \
                    if (bli_is_conj(conja))                                    \
                        PACKM_1E_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, nr / 2, \
                                      bbn, 2, scal2jris);                      \
                    else                                                       \
                        PACKM_1E_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, nr / 2, \
                                      bbn, 2, scal2ris);                       \
                } else {                                                       \
                    if (bli_is_conj(conja))                                    \
                        PACKM_1E_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, nr / 2, \
                                      bbn, inca2, scal2jris);                  \
                    else                                                       \
                        PACKM_1E_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, nr / 2, \
                                      bbn, inca2, scal2ris);                   \
                }                                                              \
            } else {                                                           \
                if (bli_is_conj(conja))                                        \
                    PACKM_1E_BODY(ctypep_r, cha, chp, , cdim, cdim_bcast,      \
                                  inca2, scal2jris);                           \
                else                                                           \
                    PACKM_1E_BODY(ctypep_r, cha, chp, , cdim, cdim_bcast,      \
                                  inca2, scal2ris);                            \
            }                                                                  \
                                                                               \
            bli_tset0s_edge(chp_r, cdim2 *cdim_bcast,                          \
                            2 * cdim_max * cdim_bcast, 2 * n, 2 * n_max,       \
                            (ctypep_r *)p, ldp);                               \
        } else {                                                               \
            const inc_t inca2 = 2 * inca;                                      \
            const inc_t lda2 = 2 * lda;                                        \
            const inc_t ldp2 = 2 * ldp;                                        \
                                                                               \
            ctypep_r kappa_r = ((ctypep_r *)kappa)[0];                         \
            ctypep_r kappa_i = ((ctypep_r *)kappa)[1];                         \
            const ctypea_r *restrict alpha1 = (ctypea_r *)a;                   \
            ctypep_r *restrict pi1_r = (ctypep_r *)p;                          \
            ctypep_r *restrict pi1_i = (ctypep_r *)p + ldp;                    \
                                                                               \
            if (cdim == mr && cdim_bcast == bbm && mr != -1) {                 \
                if (inca == 1) {                                               \
                    if (bli_is_conj(conja))                                    \
                        PACKM_1R_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, mr,     \
                                      bbm, 2, scal2jris);                      \
                    else                                                       \
                        PACKM_1R_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, mr,     \
                                      bbm, 2, scal2ris);                       \
                } else {                                                       \
                    if (bli_is_conj(conja))                                    \
                        PACKM_1R_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, mr,     \
                                      bbm, inca2, scal2jris);                  \
                    else                                                       \
                        PACKM_1R_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, mr,     \
                                      bbm, inca2, scal2ris);                   \
                }                                                              \
            } else if (cdim == nr && cdim_bcast == bbn && nr != -1) {          \
                if (inca == 1) {                                               \
                    if (bli_is_conj(conja))                                    \
                        PACKM_1R_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, nr,     \
                                      bbn, 2, scal2jris);                      \
                    else                                                       \
                        PACKM_1R_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, nr,     \
                                      bbn, 2, scal2ris);                       \
                } else {                                                       \
                    if (bli_is_conj(conja))                                    \
                        PACKM_1R_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, nr,     \
                                      bbn, inca2, scal2jris);                  \
                    else                                                       \
                        PACKM_1R_BODY(ctypep_r, cha, chp, PRAGMA_SIMD, nr,     \
                                      bbn, inca2, scal2ris);                   \
                }                                                              \
            } else {                                                           \
                if (bli_is_conj(conja))                                        \
                    PACKM_1R_BODY(ctypep_r, cha, chp, , cdim, cdim_bcast,      \
                                  inca2, scal2jris);                           \
                else                                                           \
                    PACKM_1R_BODY(ctypep_r, cha, chp, , cdim, cdim_bcast,      \
                                  inca2, scal2ris);                            \
            }                                                                  \
                                                                               \
            bli_tset0s_edge(chp_r, cdim *cdim_bcast, cdim_max *cdim_bcast,     \
                            2 * n, 2 * n_max, (ctypep_r *)p, ldp);             \
        }                                                                      \
    }

/*
 * ref_kernels/1m/bli_packm_cxk_1er_ref.c:225
 */
GENTFUNC2R(dcomplex, double, z, d, dcomplex, double, z, d, packm_1er, _generic,
           _ref)

/*
 * Wrapper for invoking the extracted kernel.
 */
void bli_zzpackm_1er_generic_ref_wrapper(
    conj_t conja, pack_t schema, dim_t cdim, dim_t cdim_max, dim_t cdim_bcast,
    dim_t n, dim_t n_max, const void *kappa, const void *a, inc_t inca,
    inc_t lda, void *p, inc_t ldp, const void *params, const cntx_t *cntx) {
    bli_zzpackm_1er_generic_ref(conja, schema, cdim, cdim_max, cdim_bcast, n,
                                n_max, kappa, a, inca, lda, p, ldp, params,
                                cntx);
}

```
Libraries
+---freetype-2.8.1
|   +---include
|   |   |   ft2build.h
|   |   |   
|   |   \---freetype
|   |       |   freetype.h
|   |       |   ftadvanc.h
|   |       |   ftautoh.h
|   |       |   ftbbox.h
|   |       |   ftbdf.h
|   |       |   ftbitmap.h
|   |       |   ftbzip2.h
|   |       |   ftcache.h
|   |       |   ftcffdrv.h
|   |       |   ftchapters.h
|   |       |   ftcid.h
|   |       |   fterrdef.h
|   |       |   fterrors.h
|   |       |   ftfntfmt.h
|   |       |   ftgasp.h
|   |       |   ftglyph.h
|   |       |   ftgxval.h
|   |       |   ftgzip.h
|   |       |   ftimage.h
|   |       |   ftincrem.h
|   |       |   ftlcdfil.h
|   |       |   ftlist.h
|   |       |   ftlzw.h
|   |       |   ftmac.h
|   |       |   ftmm.h
|   |       |   ftmodapi.h
|   |       |   ftmoderr.h
|   |       |   ftotval.h
|   |       |   ftoutln.h
|   |       |   ftpcfdrv.h
|   |       |   ftpfr.h
|   |       |   ftrender.h
|   |       |   ftsizes.h
|   |       |   ftsnames.h
|   |       |   ftstroke.h
|   |       |   ftsynth.h
|   |       |   ftsystem.h
|   |       |   fttrigon.h
|   |       |   ftttdrv.h
|   |       |   fttypes.h
|   |       |   ftwinfnt.h
|   |       |   t1tables.h
|   |       |   ttnameid.h
|   |       |   tttables.h
|   |       |   tttags.h
|   |       |   ttunpat.h
|   |       |   
|   |       +---config
|   |       |       ftconfig.h
|   |       |       ftheader.h
|   |       |       ftmodule.h
|   |       |       ftoption.h
|   |       |       ftstdlib.h
|   |       |       
|   |       \---internal
|   |           |   autohint.h
|   |           |   ftcalc.h
|   |           |   ftdebug.h
|   |           |   ftdriver.h
|   |           |   ftgloadr.h
|   |           |   fthash.h
|   |           |   ftmemory.h
|   |           |   ftobjs.h
|   |           |   ftpic.h
|   |           |   ftrfork.h
|   |           |   ftserv.h
|   |           |   ftstream.h
|   |           |   fttrace.h
|   |           |   ftvalid.h
|   |           |   internal.h
|   |           |   psaux.h
|   |           |   pshints.h
|   |           |   sfnt.h
|   |           |   t1types.h
|   |           |   tttypes.h
|   |           |   
|   |           \---services
|   |                   svbdf.h
|   |                   svcid.h
|   |                   svfntfmt.h
|   |                   svgldict.h
|   |                   svgxval.h
|   |                   svkern.h
|   |                   svmetric.h
|   |                   svmm.h
|   |                   svotval.h
|   |                   svpfr.h
|   |                   svpostnm.h
|   |                   svprop.h
|   |                   svpscmap.h
|   |                   svpsinfo.h
|   |                   svsfnt.h
|   |                   svttcmap.h
|   |                   svtteng.h
|   |                   svttglyf.h
|   |                   svwinfnt.h
|   |                   
|   \---lib
|       +---Debug
|       |       freetype.lib
|       |       freetype.pdb
|       |       
|       \---Release
|               freetype.lib
|               
+---glew-2.1.0
|   +---bin
|   |   +---Debug
|   |   |       glew32d.dll
|   |   |       glew32d.ilk
|   |   |       glew32d.pdb
|   |   |       
|   |   \---Release
|   |       |   Copy locations explained.url
|   |       |   glew32.dll
|   |       |   
|   |       \---Win32
|   |               glew32.dll
|   |               
|   +---include
|   |   \---GL
|   |           eglew.h
|   |           glew.h
|   |           glxew.h
|   |           wglew.h
|   |           
|   \---lib
|       +---Debug
|       |       glew32d.exp
|       |       glew32d.lib
|       |       libglew32d.lib
|       |       
|       \---Release
|               glew32.exp
|               glew32.lib
|               libglew32.lib
|               
+---glfw-3.2.1
|   +---bin
|   |   +---Debug
|   |   |       glfw.pdb
|   |   |       glfw3.lib
|   |   |       
|   |   \---Release
|   |           glfw3.lib
|   |           
|   \---include
|       +---glad
|       |       glad.h
|       |       
|       \---GLFW
|               glfw3.h
|               glfw3native.h
|               
\---glm-9.8.5
    \---glm
        |   common.hpp
        |   exponential.hpp
        |   ext.hpp
        |   fwd.hpp
        |   geometric.hpp
        |   glm.hpp
        |   integer.hpp
        |   mat2x2.hpp
        |   mat2x3.hpp
        |   mat2x4.hpp
        |   mat3x2.hpp
        |   mat3x3.hpp
        |   mat3x4.hpp
        |   mat4x2.hpp
        |   mat4x3.hpp
        |   mat4x4.hpp
        |   matrix.hpp
        |   packing.hpp
        |   trigonometric.hpp
        |   vec2.hpp
        |   vec3.hpp
        |   vec4.hpp
        |   vector_relational.hpp
        |   
        +---detail
        |       dummy.cpp
        |       func_common.hpp
        |       func_common.inl
        |       func_common_simd.inl
        |       func_exponential.hpp
        |       func_exponential.inl
        |       func_exponential_simd.inl
        |       func_geometric.hpp
        |       func_geometric.inl
        |       func_geometric_simd.inl
        |       func_integer.hpp
        |       func_integer.inl
        |       func_integer_simd.inl
        |       func_matrix.hpp
        |       func_matrix.inl
        |       func_matrix_simd.inl
        |       func_packing.hpp
        |       func_packing.inl
        |       func_packing_simd.inl
        |       func_trigonometric.hpp
        |       func_trigonometric.inl
        |       func_trigonometric_simd.inl
        |       func_vector_relational.hpp
        |       func_vector_relational.inl
        |       func_vector_relational_simd.inl
        |       glm.cpp
        |       precision.hpp
        |       setup.hpp
        |       type_float.hpp
        |       type_gentype.hpp
        |       type_gentype.inl
        |       type_half.hpp
        |       type_half.inl
        |       type_int.hpp
        |       type_mat.hpp
        |       type_mat.inl
        |       type_mat2x2.hpp
        |       type_mat2x2.inl
        |       type_mat2x3.hpp
        |       type_mat2x3.inl
        |       type_mat2x4.hpp
        |       type_mat2x4.inl
        |       type_mat3x2.hpp
        |       type_mat3x2.inl
        |       type_mat3x3.hpp
        |       type_mat3x3.inl
        |       type_mat3x4.hpp
        |       type_mat3x4.inl
        |       type_mat4x2.hpp
        |       type_mat4x2.inl
        |       type_mat4x3.hpp
        |       type_mat4x3.inl
        |       type_mat4x4.hpp
        |       type_mat4x4.inl
        |       type_mat4x4_simd.inl
        |       type_vec.hpp
        |       type_vec.inl
        |       type_vec1.hpp
        |       type_vec1.inl
        |       type_vec2.hpp
        |       type_vec2.inl
        |       type_vec3.hpp
        |       type_vec3.inl
        |       type_vec4.hpp
        |       type_vec4.inl
        |       type_vec4_simd.inl
        |       _features.hpp
        |       _fixes.hpp
        |       _noise.hpp
        |       _swizzle.hpp
        |       _swizzle_func.hpp
        |       _vectorize.hpp
        |       
        +---gtc
        |       bitfield.hpp
        |       bitfield.inl
        |       color_encoding.inl
        |       color_space.hpp
        |       color_space.inl
        |       constants.hpp
        |       constants.inl
        |       epsilon.hpp
        |       epsilon.inl
        |       functions.hpp
        |       functions.inl
        |       integer.hpp
        |       integer.inl
        |       matrix_access.hpp
        |       matrix_access.inl
        |       matrix_integer.hpp
        |       matrix_inverse.hpp
        |       matrix_inverse.inl
        |       matrix_transform.hpp
        |       matrix_transform.inl
        |       noise.hpp
        |       noise.inl
        |       packing.hpp
        |       packing.inl
        |       quaternion.hpp
        |       quaternion.inl
        |       quaternion_simd.inl
        |       random.hpp
        |       random.inl
        |       reciprocal.hpp
        |       reciprocal.inl
        |       round.hpp
        |       round.inl
        |       type_aligned.hpp
        |       type_precision.hpp
        |       type_precision.inl
        |       type_ptr.hpp
        |       type_ptr.inl
        |       ulp.hpp
        |       ulp.inl
        |       vec1.hpp
        |       vec1.inl
        |       
        +---gtx
        |       associated_min_max.hpp
        |       associated_min_max.inl
        |       bit.hpp
        |       bit.inl
        |       closest_point.hpp
        |       closest_point.inl
        |       color_space.hpp
        |       color_space.inl
        |       color_space_YCoCg.hpp
        |       color_space_YCoCg.inl
        |       common.hpp
        |       common.inl
        |       compatibility.hpp
        |       compatibility.inl
        |       component_wise.hpp
        |       component_wise.inl
        |       dual_quaternion.hpp
        |       dual_quaternion.inl
        |       euler_angles.hpp
        |       euler_angles.inl
        |       extend.hpp
        |       extend.inl
        |       extended_min_max.hpp
        |       extended_min_max.inl
        |       fast_exponential.hpp
        |       fast_exponential.inl
        |       fast_square_root.hpp
        |       fast_square_root.inl
        |       fast_trigonometry.hpp
        |       fast_trigonometry.inl
        |       float_notmalize.inl
        |       gradient_paint.hpp
        |       gradient_paint.inl
        |       handed_coordinate_space.hpp
        |       handed_coordinate_space.inl
        |       hash.hpp
        |       hash.inl
        |       integer.hpp
        |       integer.inl
        |       intersect.hpp
        |       intersect.inl
        |       io.hpp
        |       io.inl
        |       log_base.hpp
        |       log_base.inl
        |       matrix_cross_product.hpp
        |       matrix_cross_product.inl
        |       matrix_decompose.hpp
        |       matrix_decompose.inl
        |       matrix_interpolation.hpp
        |       matrix_interpolation.inl
        |       matrix_major_storage.hpp
        |       matrix_major_storage.inl
        |       matrix_operation.hpp
        |       matrix_operation.inl
        |       matrix_query.hpp
        |       matrix_query.inl
        |       matrix_transform_2d.hpp
        |       matrix_transform_2d.inl
        |       mixed_product.hpp
        |       mixed_product.inl
        |       norm.hpp
        |       norm.inl
        |       normal.hpp
        |       normal.inl
        |       normalize_dot.hpp
        |       normalize_dot.inl
        |       number_precision.hpp
        |       number_precision.inl
        |       optimum_pow.hpp
        |       optimum_pow.inl
        |       orthonormalize.hpp
        |       orthonormalize.inl
        |       perpendicular.hpp
        |       perpendicular.inl
        |       polar_coordinates.hpp
        |       polar_coordinates.inl
        |       projection.hpp
        |       projection.inl
        |       quaternion.hpp
        |       quaternion.inl
        |       range.hpp
        |       raw_data.hpp
        |       raw_data.inl
        |       rotate_normalized_axis.hpp
        |       rotate_normalized_axis.inl
        |       rotate_vector.hpp
        |       rotate_vector.inl
        |       scalar_multiplication.hpp
        |       scalar_relational.hpp
        |       scalar_relational.inl
        |       spline.hpp
        |       spline.inl
        |       std_based_type.hpp
        |       std_based_type.inl
        |       string_cast.hpp
        |       string_cast.inl
        |       transform.hpp
        |       transform.inl
        |       transform2.hpp
        |       transform2.inl
        |       type_aligned.hpp
        |       type_aligned.inl
        |       type_trait.hpp
        |       type_trait.inl
        |       vector_angle.hpp
        |       vector_angle.inl
        |       vector_query.hpp
        |       vector_query.inl
        |       wrap.hpp
        |       wrap.inl
        |       
        \---simd
                common.h
                exponential.h
                geometric.h
                integer.h
                matrix.h
                packing.h
                platform.h
                trigonometric.h
                vector_relational.h             
```

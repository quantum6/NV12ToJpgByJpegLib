 
int clipNv12ToJpgFile(const char *pFileName,
    const char* pYUVBuffer, const int nWidth, const int nHeight)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];  
    FILE * pJpegFile = NULL;
    unsigned char *yuvbuf = NULL;
    unsigned char *ybase = NULL, *ubase = NULL;
    int i=0, j=0;
    int idx=0;
 
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
 
    if ((pJpegFile = fopen(pFileName, "wb")) == NULL)
    {    
        return -1;
    }    
    jpeg_stdio_dest(&cinfo, pJpegFile);
 
    // image width and height, in pixels
    cinfo.image_width      = nWidth;
    cinfo.image_height     = nHeight;    
    cinfo.input_components = 3;    // # of color components per pixel
    cinfo.in_color_space   = JCS_YCbCr;  //colorspace of input image
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, JPEG_QUALITY, TRUE );
  
    cinfo.jpeg_color_space = JCS_YCbCr;
    cinfo.comp_info[0].h_samp_factor = 2;
    cinfo.comp_info[0].v_samp_factor = 2;
  
    jpeg_start_compress(&cinfo, TRUE);
  
    if(!(yuvbuf=(unsigned char *)malloc(nWidth*3))!=NULL)
    {
        return -1;
    }
    memset(yuvbuf, 0, nWidth*3);
  
    ybase=pYUVBuffer;
    ubase=pYUVBuffer+nWidth*nHeight;
    while (cinfo.next_scanline < cinfo.image_height)
    {
        idx=0;
        for(i=0;i<nWidth;i++)
        {   
            yuvbuf[idx++]=ybase[i + j * nWidth];
            yuvbuf[idx++]=ubase[j/2 * nWidth+(i/2)*2];
            yuvbuf[idx++]=ubase[j/2 * nWidth+(i/2)*2+1];
        }  
        row_pointer[0] = yuvbuf;
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
        j++;
    }
    jpeg_finish_compress( &cinfo);
 
    jpeg_destroy_compress(&cinfo);
    fclose(pJpegFile);
 
    return 0;    
}

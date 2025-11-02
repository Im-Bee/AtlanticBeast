// --------------------------------------------------------------------------------------------------------------------
bool IsCrosshair(in const vec2 uv, 
                 in const float aspectRatio) 
{
    const float fCrossHairWidth = 0.0026;
    float fCrossHairWidthX = fCrossHairWidth;
    float fCrossHairWidthY = fCrossHairWidth;

    if (aspectRatio >= 1.f) {
        fCrossHairWidthY *= aspectRatio;
    } else {
        fCrossHairWidthX *= aspectRatio;
    }

    return abs(uv).x < fCrossHairWidthX && abs(uv).y < fCrossHairWidthY;
}


#pragma once
class Visuals
{

public:

	void glowPlayers (uint32_t glowType, float chamsColor[4]);
	void renderWithImgui ( );
	void NoSmoke ( );
	void changeSmokeColor (float smokeColor[3]);
};



inline Visuals *iVisuals = new Visuals ( );

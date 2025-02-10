#pragma once
class Visuals
{

public:

	void glowPlayers ( );
	void renderWithImgui ( );
	void NoSmoke ( );
	void changeSmokeColor (float smokeColor[3]);
};

inline Visuals *VisualsManager = new Visuals ( );

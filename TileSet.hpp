class TileSet {
	public:

		bool load(const char* buf, int len);
		int size();
		bool save(const char* buf, int len);

		bool isSolid(int tile, int side) const;
		int surfaceType(int tile, int side) const;
		int behavior(int tile) const;
		bool isSloped(int tile) const;
		int slopedSide(int tile) const;
		int slope(int tile) const;
		int slopeOffset(int tile) const;
		int tileOffset(int tile, int frame) const;

		void setSolid(bool, int tile, int side);
		void setsurfaceType(int, int tile, int side);
		void setbehavior(int, int tile);
		void setSloped(bool, int tile);
		void setSlopedSide(int, int tile);
		void setSlope(int, int tile);
		void setSlopeOffset(int, int tile);
		void setTileOffset(int, int tile, int frame);

	private:
		int surfaces[910][4];
		int behavior[910];
		int frames[910][8];
		int frameCount;


};

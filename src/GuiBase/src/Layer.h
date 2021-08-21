#pragma once


namespace SynGUI {

	class Layer;

	class SortOrder {
	private:
		float zDepth = -1.0f;
		int sortOrder = 0;
		SortOrder();
		~SortOrder();
	public:
		void setLayer(Layer);
		void setSortOrder(int i);
		float getZDepth();
	};

	class Layer {
	private:
		int layerOrder = 0;
	public:
		Layer();
		~Layer();

		Layer putInFrontOf(Layer layer);
		Layer putInBackOf(Layer layer);

		SortOrder sortOrder(int i);
	};
}
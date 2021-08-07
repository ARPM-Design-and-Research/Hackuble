//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     IconData data = nlohmann::json::parse(jsonString);

#pragma once

#include "nlohmann/json.hpp"

#include <optional>
#include <stdexcept>
#include <regex>

/* Used to parse JSON data for the iconRenderer and textRenderer
*/

namespace quicktype {
    using nlohmann::json;

    /*inline json get_untyped(const json& j, const char* property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json& j, std::string property) {
        return get_untyped(j, property.data());
    }*/

    class SpriteSourceSizeClass {
    public:
        SpriteSourceSizeClass() = default;
        virtual ~SpriteSourceSizeClass() = default;

    private:
        int64_t x;
        int64_t y;
        int64_t w;
        int64_t h;

    public:
        const int64_t& get_x() const { return x; }
        int64_t& get_mutable_x() { return x; }
        void set_x(const int64_t& value) { this->x = value; }

        const int64_t& get_y() const { return y; }
        int64_t& get_mutable_y() { return y; }
        void set_y(const int64_t& value) { this->y = value; }

        const int64_t& get_w() const { return w; }
        int64_t& get_mutable_w() { return w; }
        void set_w(const int64_t& value) { this->w = value; }

        const int64_t& get_h() const { return h; }
        int64_t& get_mutable_h() { return h; }
        void set_h(const int64_t& value) { this->h = value; }
    };

    class Size {
    public:
        Size() = default;
        virtual ~Size() = default;

    private:
        int64_t w;
        int64_t h;

    public:
        const int64_t& get_w() const { return w; }
        int64_t& get_mutable_w() { return w; }
        void set_w(const int64_t& value) { this->w = value; }

        const int64_t& get_h() const { return h; }
        int64_t& get_mutable_h() { return h; }
        void set_h(const int64_t& value) { this->h = value; }
    };

    class FrameValue {
    public:
        FrameValue() = default;
        virtual ~FrameValue() = default;

    private:
        SpriteSourceSizeClass frame;
        bool rotated;
        bool trimmed;
        Size source_size;
        SpriteSourceSizeClass sprite_source_size;

    public:
        const SpriteSourceSizeClass& get_frame() const { return frame; }
        SpriteSourceSizeClass& get_mutable_frame() { return frame; }
        void set_frame(const SpriteSourceSizeClass& value) { this->frame = value; }

        const bool& get_rotated() const { return rotated; }
        bool& get_mutable_rotated() { return rotated; }
        void set_rotated(const bool& value) { this->rotated = value; }

        const bool& get_trimmed() const { return trimmed; }
        bool& get_mutable_trimmed() { return trimmed; }
        void set_trimmed(const bool& value) { this->trimmed = value; }

        const Size& get_source_size() const { return source_size; }
        Size& get_mutable_source_size() { return source_size; }
        void set_source_size(const Size& value) { this->source_size = value; }

        const SpriteSourceSizeClass& get_sprite_source_size() const { return sprite_source_size; }
        SpriteSourceSizeClass& get_mutable_sprite_source_size() { return sprite_source_size; }
        void set_sprite_source_size(const SpriteSourceSizeClass& value) { this->sprite_source_size = value; }
    };

    class Meta {
    public:
        Meta() = default;
        virtual ~Meta() = default;

    private:
        std::string app;
        std::string image;
        Size size;

    public:
        const std::string& get_app() const { return app; }
        std::string& get_mutable_app() { return app; }
        void set_app(const std::string& value) { this->app = value; }

        const std::string& get_image() const { return image; }
        std::string& get_mutable_image() { return image; }
        void set_image(const std::string& value) { this->image = value; }

        const Size& get_size() const { return size; }
        Size& get_mutable_size() { return size; }
        void set_size(const Size& value) { this->size = value; }
    };

    class IconData {
    public:
        IconData() = default;
        virtual ~IconData() = default;

    private:
        Meta meta;
        std::map<std::string, FrameValue> frames;

    public:
        const Meta& get_meta() const { return meta; }
        Meta& get_mutable_meta() { return meta; }
        void set_meta(const Meta& value) { this->meta = value; }

        const std::map<std::string, FrameValue>& get_frames() const { return frames; }
        std::map<std::string, FrameValue>& get_mutable_frames() { return frames; }
        void set_frames(const std::map<std::string, FrameValue>& value) { this->frames = value; }
    };
}

namespace nlohmann {
    void from_json(const json& j, quicktype::SpriteSourceSizeClass& x);
    void to_json(json& j, const quicktype::SpriteSourceSizeClass& x);

    void from_json(const json& j, quicktype::Size& x);
    void to_json(json& j, const quicktype::Size& x);

    void from_json(const json& j, quicktype::FrameValue& x);
    void to_json(json& j, const quicktype::FrameValue& x);

    void from_json(const json& j, quicktype::Meta& x);
    void to_json(json& j, const quicktype::Meta& x);

    void from_json(const json& j, quicktype::IconData& x);
    void to_json(json& j, const quicktype::IconData& x);

    inline void from_json(const json& j, quicktype::SpriteSourceSizeClass& x) {
        x.set_x(j.at("x").get<int64_t>());
        x.set_y(j.at("y").get<int64_t>());
        x.set_w(j.at("w").get<int64_t>());
        x.set_h(j.at("h").get<int64_t>());
    }

    inline void to_json(json& j, const quicktype::SpriteSourceSizeClass& x) {
        j = json::object();
        j["x"] = x.get_x();
        j["y"] = x.get_y();
        j["w"] = x.get_w();
        j["h"] = x.get_h();
    }

    inline void from_json(const json& j, quicktype::Size& x) {
        x.set_w(j.at("w").get<int64_t>());
        x.set_h(j.at("h").get<int64_t>());
    }

    inline void to_json(json& j, const quicktype::Size& x) {
        j = json::object();
        j["w"] = x.get_w();
        j["h"] = x.get_h();
    }

    inline void from_json(const json& j, quicktype::FrameValue& x) {
        x.set_frame(j.at("frame").get<quicktype::SpriteSourceSizeClass>());
        x.set_rotated(j.at("rotated").get<bool>());
        x.set_trimmed(j.at("trimmed").get<bool>());
        x.set_source_size(j.at("sourceSize").get<quicktype::Size>());
        x.set_sprite_source_size(j.at("spriteSourceSize").get<quicktype::SpriteSourceSizeClass>());
    }

    inline void to_json(json& j, const quicktype::FrameValue& x) {
        j = json::object();
        j["frame"] = x.get_frame();
        j["rotated"] = x.get_rotated();
        j["trimmed"] = x.get_trimmed();
        j["sourceSize"] = x.get_source_size();
        j["spriteSourceSize"] = x.get_sprite_source_size();
    }

    inline void from_json(const json& j, quicktype::Meta& x) {
        x.set_app(j.at("app").get<std::string>());
        x.set_image(j.at("image").get<std::string>());
        x.set_size(j.at("size").get<quicktype::Size>());
    }

    inline void to_json(json& j, const quicktype::Meta& x) {
        j = json::object();
        j["app"] = x.get_app();
        j["image"] = x.get_image();
        j["size"] = x.get_size();
    }

    inline void from_json(const json& j, quicktype::IconData& x) {
        x.set_meta(j.at("meta").get<quicktype::Meta>());
        x.set_frames(j.at("frames").get<std::map<std::string, quicktype::FrameValue>>());
    }

    inline void to_json(json& j, const quicktype::IconData& x) {
        j = json::object();
        j["meta"] = x.get_meta();
        j["frames"] = x.get_frames();
    }
}


#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>> {
        static void to_json(json& j, const std::shared_ptr<T>& opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::shared_ptr<T> from_json(const json& j) {
            if (j.is_null()) return std::unique_ptr<T>(); else return std::unique_ptr<T>(new T(j.get<T>()));
        }
    };
}
#endif

namespace quicktype {
    using nlohmann::json;

    inline json get_untyped(const json& j, const char* property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json& j, std::string property) {
        return get_untyped(j, property.data());
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json& j, const char* property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_optional(const json& j, std::string property) {
        return get_optional<T>(j, property.data());
    }

    class Atlas {
    public:
        Atlas() = default;
        virtual ~Atlas() = default;

    private:
        std::string type;
        int64_t distance_range;
        int64_t size;
        int64_t width;
        int64_t height;
        std::string y_origin;

    public:
        const std::string& get_type() const { return type; }
        std::string& get_mutable_type() { return type; }
        void set_type(const std::string& value) { this->type = value; }

        const int64_t& get_distance_range() const { return distance_range; }
        int64_t& get_mutable_distance_range() { return distance_range; }
        void set_distance_range(const int64_t& value) { this->distance_range = value; }

        const int64_t& get_size() const { return size; }
        int64_t& get_mutable_size() { return size; }
        void set_size(const int64_t& value) { this->size = value; }

        const int64_t& get_width() const { return width; }
        int64_t& get_mutable_width() { return width; }
        void set_width(const int64_t& value) { this->width = value; }

        const int64_t& get_height() const { return height; }
        int64_t& get_mutable_height() { return height; }
        void set_height(const int64_t& value) { this->height = value; }

        const std::string& get_y_origin() const { return y_origin; }
        std::string& get_mutable_y_origin() { return y_origin; }
        void set_y_origin(const std::string& value) { this->y_origin = value; }
    };

    class Bounds {
    public:
        Bounds() = default;
        virtual ~Bounds() = default;

    private:
        double left;
        double bottom;
        double right;
        double top;

    public:
        const double& get_left() const { return left; }
        double& get_mutable_left() { return left; }
        void set_left(const double& value) { this->left = value; }

        const double& get_bottom() const { return bottom; }
        double& get_mutable_bottom() { return bottom; }
        void set_bottom(const double& value) { this->bottom = value; }

        const double& get_right() const { return right; }
        double& get_mutable_right() { return right; }
        void set_right(const double& value) { this->right = value; }

        const double& get_top() const { return top; }
        double& get_mutable_top() { return top; }
        void set_top(const double& value) { this->top = value; }
    };

    class Glyph {
    public:
        Glyph() = default;
        virtual ~Glyph() = default;

    private:
        int64_t unicode;
        double advance;
        std::shared_ptr<Bounds> plane_bounds;
        std::shared_ptr<Bounds> atlas_bounds;

    public:
        const int64_t& get_unicode() const { return unicode; }
        int64_t& get_mutable_unicode() { return unicode; }
        void set_unicode(const int64_t& value) { this->unicode = value; }

        const double& get_advance() const { return advance; }
        double& get_mutable_advance() { return advance; }
        void set_advance(const double& value) { this->advance = value; }

        std::shared_ptr<Bounds> get_plane_bounds() const { return plane_bounds; }
        void set_plane_bounds(std::shared_ptr<Bounds> value) { this->plane_bounds = value; }

        std::shared_ptr<Bounds> get_atlas_bounds() const { return atlas_bounds; }
        void set_atlas_bounds(std::shared_ptr<Bounds> value) { this->atlas_bounds = value; }
    };

    class Metrics {
    public:
        Metrics() = default;
        virtual ~Metrics() = default;

    private:
        int64_t em_size;
        double line_height;
        double ascender;
        double descender;
        double underline_y;
        double underline_thickness;

    public:
        const int64_t& get_em_size() const { return em_size; }
        int64_t& get_mutable_em_size() { return em_size; }
        void set_em_size(const int64_t& value) { this->em_size = value; }

        const double& get_line_height() const { return line_height; }
        double& get_mutable_line_height() { return line_height; }
        void set_line_height(const double& value) { this->line_height = value; }

        const double& get_ascender() const { return ascender; }
        double& get_mutable_ascender() { return ascender; }
        void set_ascender(const double& value) { this->ascender = value; }

        const double& get_descender() const { return descender; }
        double& get_mutable_descender() { return descender; }
        void set_descender(const double& value) { this->descender = value; }

        const double& get_underline_y() const { return underline_y; }
        double& get_mutable_underline_y() { return underline_y; }
        void set_underline_y(const double& value) { this->underline_y = value; }

        const double& get_underline_thickness() const { return underline_thickness; }
        double& get_mutable_underline_thickness() { return underline_thickness; }
        void set_underline_thickness(const double& value) { this->underline_thickness = value; }
    };

    class FontData {
    public:
        FontData() = default;
        virtual ~FontData() = default;

    private:
        Atlas atlas;
        Metrics metrics;
        std::vector<Glyph> glyphs;
        std::vector<nlohmann::json> kerning;

    public:
        const Atlas& get_atlas() const { return atlas; }
        Atlas& get_mutable_atlas() { return atlas; }
        void set_atlas(const Atlas& value) { this->atlas = value; }

        const Metrics& get_metrics() const { return metrics; }
        Metrics& get_mutable_metrics() { return metrics; }
        void set_metrics(const Metrics& value) { this->metrics = value; }

        const std::vector<Glyph>& get_glyphs() const { return glyphs; }
        std::vector<Glyph>& get_mutable_glyphs() { return glyphs; }
        void set_glyphs(const std::vector<Glyph>& value) { this->glyphs = value; }

        const std::vector<nlohmann::json>& get_kerning() const { return kerning; }
        std::vector<nlohmann::json>& get_mutable_kerning() { return kerning; }
        void set_kerning(const std::vector<nlohmann::json>& value) { this->kerning = value; }
    };
}

namespace nlohmann {
    void from_json(const json& j, quicktype::Atlas& x);
    void to_json(json& j, const quicktype::Atlas& x);

    void from_json(const json& j, quicktype::Bounds& x);
    void to_json(json& j, const quicktype::Bounds& x);

    void from_json(const json& j, quicktype::Glyph& x);
    void to_json(json& j, const quicktype::Glyph& x);

    void from_json(const json& j, quicktype::Metrics& x);
    void to_json(json& j, const quicktype::Metrics& x);

    void from_json(const json& j, quicktype::FontData& x);
    void to_json(json& j, const quicktype::FontData& x);

    inline void from_json(const json& j, quicktype::Atlas& x) {
        x.set_type(j.at("type").get<std::string>());
        x.set_distance_range(j.at("distanceRange").get<int64_t>());
        x.set_size(j.at("size").get<int64_t>());
        x.set_width(j.at("width").get<int64_t>());
        x.set_height(j.at("height").get<int64_t>());
        x.set_y_origin(j.at("yOrigin").get<std::string>());
    }

    inline void to_json(json& j, const quicktype::Atlas& x) {
        j = json::object();
        j["type"] = x.get_type();
        j["distanceRange"] = x.get_distance_range();
        j["size"] = x.get_size();
        j["width"] = x.get_width();
        j["height"] = x.get_height();
        j["yOrigin"] = x.get_y_origin();
    }

    inline void from_json(const json& j, quicktype::Bounds& x) {
        x.set_left(j.at("left").get<double>());
        x.set_bottom(j.at("bottom").get<double>());
        x.set_right(j.at("right").get<double>());
        x.set_top(j.at("top").get<double>());
    }

    inline void to_json(json& j, const quicktype::Bounds& x) {
        j = json::object();
        j["left"] = x.get_left();
        j["bottom"] = x.get_bottom();
        j["right"] = x.get_right();
        j["top"] = x.get_top();
    }

    inline void from_json(const json& j, quicktype::Glyph& x) {
        x.set_unicode(j.at("unicode").get<int64_t>());
        x.set_advance(j.at("advance").get<double>());
        x.set_plane_bounds(quicktype::get_optional<quicktype::Bounds>(j, "planeBounds"));
        x.set_atlas_bounds(quicktype::get_optional<quicktype::Bounds>(j, "atlasBounds"));
    }

    inline void to_json(json& j, const quicktype::Glyph& x) {
        j = json::object();
        j["unicode"] = x.get_unicode();
        j["advance"] = x.get_advance();
        j["planeBounds"] = x.get_plane_bounds();
        j["atlasBounds"] = x.get_atlas_bounds();
    }

    inline void from_json(const json& j, quicktype::Metrics& x) {
        x.set_em_size(j.at("emSize").get<int64_t>());
        x.set_line_height(j.at("lineHeight").get<double>());
        x.set_ascender(j.at("ascender").get<double>());
        x.set_descender(j.at("descender").get<double>());
        x.set_underline_y(j.at("underlineY").get<double>());
        x.set_underline_thickness(j.at("underlineThickness").get<double>());
    }

    inline void to_json(json& j, const quicktype::Metrics& x) {
        j = json::object();
        j["emSize"] = x.get_em_size();
        j["lineHeight"] = x.get_line_height();
        j["ascender"] = x.get_ascender();
        j["descender"] = x.get_descender();
        j["underlineY"] = x.get_underline_y();
        j["underlineThickness"] = x.get_underline_thickness();
    }

    inline void from_json(const json& j, quicktype::FontData& x) {
        x.set_atlas(j.at("atlas").get<quicktype::Atlas>());
        x.set_metrics(j.at("metrics").get<quicktype::Metrics>());
        x.set_glyphs(j.at("glyphs").get<std::vector<quicktype::Glyph>>());
        x.set_kerning(j.at("kerning").get<std::vector<json>>());
    }

    inline void to_json(json& j, const quicktype::FontData& x) {
        j = json::object();
        j["atlas"] = x.get_atlas();
        j["metrics"] = x.get_metrics();
        j["glyphs"] = x.get_glyphs();
        j["kerning"] = x.get_kerning();
    }
}
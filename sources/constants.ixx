export module constants;

export namespace RT::Constants {

constexpr int SCREEN_WIDTH{1200};
constexpr int SCREEN_HEIGHT{900};
constexpr int PIXELS{SCREEN_WIDTH * SCREEN_HEIGHT};

constexpr unsigned int COLOR_BLACK{0x00000000};
constexpr unsigned int COLOR_YELLOW{0xFFFF00FF};

constexpr int RAYS_NUMBER{1000};

}  // namespace RT::Constants
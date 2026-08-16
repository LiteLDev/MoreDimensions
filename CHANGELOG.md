# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.14.1] - 2026-08-16

### Fix

- fix 'CustomDimensionConfig' build error

## [0.14.0] - 2026-08-16

### Changed

- Use script api registry custom dimension
- Add [`addSimpleDimension`](https://github.com/LiteLDev/MoreDimensions/blob/v0.14.0/src/more_dimensions/api/dimension/CustomDimensionManager.h#L80) api
- Remove the export of `SimpleCustomDimension`.

## [0.13.0] - 2026-02-03

### Changed

- Supported LeviLamina 1.9.2(Server 1.21.132.3)
- Resolved some warnings in code

## [0.13.0-rc.1] - 2026-01-20

### Added

- Added support for client

### Changed

- Adapt to LeviLamina 1.8.0-rc.2(Minecraft 1.21.124.2)

## [0.12.2] - 2025-12-03

### Fixed

- Fixed crash of tests [#21]

## [0.12.1] - 2025-12-01

### Changed

- Adapt to LeviLamina 1.7.6(BDS-1.21.124.2)

## [0.12.0] - 2025-12-01

### Changed

- Adapt to LeviLamina 1.7.0(BDS-1.21.120.4)

## [0.11.2] - 2025-11-02

### Fixed

- Fixed flat type generator village dimension test crash

## [0.11.1] - 2025-10-16

### Fixed

- Fixed crash

## [0.11.0] - 2025-10-10

### Changed

- Adapt to LeviLamina 1.6.0(BDS-1.21.111.1)

## [0.10.1] - 2025-10-04

### Changed

- Adapt to LeviLamina 1.5.2(BDS-1.21.102.1)

## [0.9.0] - 2025-07-17

### Changed

- Adapt to LeviLamina 1.4.1(BDS-1.21.93.1)

## [0.8.0] - 2025-06-11

### Changed

- Adapt to LeviLamina 1.3.0(BDS-1.21.80.3)

## [0.7.0] - 2025-06-02

### Changed

- Adapt to LeviLamina 1.2.0(BDS-1.21.70.04)
- Deprecate `base64Nbt` in `dimension_config.json` files and add `sNbt` configuration. Improve the readability of configuration files

## [0.6.1] - 2025-03-05

### Fixed

- Fixed tooth

## [0.6.0] - 2025-03-04

### Changed

- Adapt to LeviLamina 1.1.0

## [0.5.0] - 2025-02-01

### Changed

- Adapt to LeviLamina 1.0.0

## [0.4.1] - 2024-07-24

### Changed

- Adapt to LeviLamina 0.13.4

## [0.4.0] - 2024-06-19

### Added

- Adapt to LeviLamina 0.13.x

## [0.3.1] - 2024-06-02

### Fixed

- Fix vanilla dimension can't create when the number of dimensions exceeds nine.

## [0.3.0] - 2024-06-01

### Added

- Adapted to LeviLamina `0.12.x`

## [0.2.1] - 2024-04-13

### Added

- Adapted to LeviLamina `0.11.x`

## [0.2.0] - 2024-03-27

### Added

- Adapted to BDS version `1.20.7x`
- Support Levilamina `v0.10.x`

### Changed

- Changed namespace `ll::dimension` to `more_dimensions`.
- Use new plugin template.

### Fixed

- Fix `PropertiesSettings::isClientSideGenEnabled` hook no trigger.

## [0.1.2] - 2024-03-10

### Fixed

- Fix: fix api symbols exports error

## [0.1.1] - 2024-03-10

### Fixed

- Fix: fix xmake packages install directory no files

## [0.1.0] - 2024-03-07

### Added

- Release the first version

[#21]: https://github.com/LiteLDev/MoreDimensions/issues/21

[Unreleased]: https://github.com/LiteLDev/MoreDimensions/compare/v0.14.0...HEAD
[0.14.0]: https://github.com/LiteLDev/MoreDimensions/compare/v0.13.0...v0.14.0
[0.13.0]: https://github.com/LiteLDev/MoreDimensions/compare/v0.13.0-rc.1...v0.13.0
[0.13.0-rc.1]: https://github.com/LiteLDev/MoreDimensions/compare/v0.12.2...v0.13.0-rc.1
[0.12.2]: https://github.com/LiteLDev/MoreDimensions/compare/v0.12.1...v0.12.2
[0.12.1]: https://github.com/LiteLDev/MoreDimensions/compare/v0.12.0...v0.12.1
[0.12.0]: https://github.com/LiteLDev/MoreDimensions/compare/v0.11.2...v0.12.0
[0.11.2]: https://github.com/LiteLDev/MoreDimensions/compare/v0.11.1...v0.11.2
[0.11.1]: https://github.com/LiteLDev/MoreDimensions/compare/v0.11.0...v0.11.1
[0.11.0]: https://github.com/LiteLDev/MoreDimensions/compare/v0.10.1...v0.11.0
[0.10.1]: https://github.com/LiteLDev/MoreDimensions/compare/v0.9.0...v0.10.1
[0.9.0]: https://github.com/LiteLDev/MoreDimensions/compare/v0.8.0...v0.9.0
[0.8.0]: https://github.com/LiteLDev/MoreDimensions/compare/v0.7.0...v0.8.0
[0.7.0]: https://github.com/LiteLDev/MoreDimensions/compare/v0.6.1...v0.7.0
[0.6.1]: https://github.com/LiteLDev/MoreDimensions/compare/v0.6.0...v0.6.1
[0.6.0]: https://github.com/LiteLDev/MoreDimensions/compare/v0.5.0...v0.6.0
[0.5.0]: https://github.com/LiteLDev/MoreDimensions/compare/v0.4.1...v0.5.0
[0.4.1]: https://github.com/LiteLDev/MoreDimensions/compare/v0.4.0...v0.4.1
[0.4.0]: https://github.com/LiteLDev/MoreDimensions/compare/v0.3.1...v0.4.0
[0.3.1]: https://github.com/LiteLDev/MoreDimensions/compare/v0.3.0...v0.3.1
[0.3.0]: https://github.com/LiteLDev/MoreDimensions/compare/v0.2.1...v0.3.0
[0.2.1]: https://github.com/LiteLDev/MoreDimensions/compare/v0.2.0...v0.2.1
[0.2.0]: https://github.com/LiteLDev/MoreDimensions/compare/v0.1.2...v0.2.0
[0.1.2]: https://github.com/LiteLDev/MoreDimensions/compare/v0.1.1...v0.1.2
[0.1.1]: https://github.com/LiteLDev/MoreDimensions/compare/v0.1.0...v0.1.1
[0.1.0]: https://github.com/LiteLDev/MoreDimensions/releases/tag/v0.1.0

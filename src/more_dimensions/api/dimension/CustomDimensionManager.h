#pragma once

#include "more_dimensions/core/Macros.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "mc/world/level/GeneratorType.h"
#include "mc/world/level/dimension/DimensionType.h"

class Dimension;
class DerivedDimensionArguments;

namespace more_dimensions {

struct DimensionFactoryInfo {
    DerivedDimensionArguments& arguments;
    CompoundTag const&         data;
    DimensionType              dimId;
};

class CustomDimensionManager {
    struct Impl;
    std::unique_ptr<Impl> impl;

    CustomDimensionManager();
    ~CustomDimensionManager();

public:
    using DimensionFactoryT = std::shared_ptr<Dimension>(DimensionFactoryInfo const&);

protected:
    MORE_DIMENSIONS_API DimensionType addDimension(
        std::string const&                  dimName,
        std::function<DimensionFactoryT>    factory,
        std::function<CompoundTag()> const& newData
    );

public:
    MORE_DIMENSIONS_API static CustomDimensionManager& getInstance();

    [[deprecated("please use VanillaDimensions::fromString")]] MORE_DIMENSIONS_API static DimensionType
    getDimensionIdFromName(std::string const& dimName);

    /**
     *
     * @brief Register a custom dimension implemented by a user-defined Dimension subclass.
     * @details The template type D must derive from Dimension and provide a constructor
     * D(std::string const&, more_dimensions::DimensionFactoryInfo const&) plus a static
     * generateNewData(Args&&...) method that returns the persisted dimension data as a CompoundTag.
     * The dimension name must be in namespace:name format and must not use the minecraft namespace.
     * @tparam D The Dimension subclass to create when the dimension is instantiated.
     * @tparam Args Argument types forwarded to D::generateNewData(...).
     * @param dimName Dimension name, unique identifier; the same string cannot be used for different dimensions.
     * @param args Arguments forwarded to D::generateNewData(...) to build the persisted dimension data.
     * @return The registered DimensionType (dimension id) of the new custom dimension.
     * @note Requires ll::service::getLevel() to be available, otherwise std::runtime_error is thrown.
     * @note If you want to use SimpleCustomDimension, please use CustomDimensionManager::addSimpleDimension.
     * @throw std::runtime_error If the level is unavailable or the dimension name is invalid.
     */
    template <std::derived_from<Dimension> D, class... Args>
    DimensionType addDimension(std::string const& dimName, Args&&... args) {
        return addDimension(
            dimName,
            [dimName](more_dimensions::DimensionFactoryInfo const& info) -> std::shared_ptr<Dimension> {
                return std::make_shared<D>(dimName, info);
            },
            [&] { return D::generateNewData(std::forward<Args>(args)...); }
        );
    }

    /**
     *
     * @brief Creating a custom dimension using SimpleCustomDimension
     * @details This is a sample dimension created using MoreDimension's Simple. If you need more configuration, please
     * refer to SimpleCustomDimension to create a Dimension class to create dimensions.
     * @param dimName Dimension name, Unique identifier; the same string cannot be used for different dimensions.
     * @param seed Dimension seed, only work for terrain generation, not for structure generation, such as
     * villages.
     * @param generatorType Generator type, using vanilla generators.
     */
    MORE_DIMENSIONS_API DimensionType addSimpleDimension(
        std::string const& dimName,
        uint               seed          = 123,
        GeneratorType      generatorType = GeneratorType::Overworld
    );
};

} // namespace more_dimensions

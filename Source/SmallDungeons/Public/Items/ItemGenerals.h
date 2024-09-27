UENUM(BlueprintType)
enum class EItemType : uint8
{
	// Unknown type
	Invalid = 0,
	// Types of items that remain in the level (e.g. doors, chests)
	Static = 1 << 0,
	// Types of items that can be removed (e.g. weapons, consumables)
	Dynamic = 1 << 1,
};
#ifndef AB_WRAPPER_H
#define AB_WRAPPER_H

namespace Voxels 
{

template<class Derived>
class IAdapter 
{
public:

    IAdapter() = default;

    ~IAdapter() = default;

public:

    IAdapter(IAdapter&&) = default;
    IAdapter(const IAdapter&) = default;

    IAdapter& operator=(IAdapter&&) noexcept = default;
    IAdapter& operator=(const IAdapter&) noexcept = default;

public:

    const ::std::vector<const char*>& GetExtensions() const
    { return static_cast<const Derived*>(this)->GetExtensionsImpl(); }

    void* GetFeatures() const
    { return static_cast<const Derived*>(this)->GetFeaturesImpl(); }

};

} // !Voxels

#endif // !AB_WRAPPER_H

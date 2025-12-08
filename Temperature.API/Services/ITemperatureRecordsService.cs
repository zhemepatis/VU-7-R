using Temperature.API.Models;

namespace Temperature.API.Services;

public interface ITemperatureRecordsService
{
    public void Add(AddTemperatureRecordRequest request);
}
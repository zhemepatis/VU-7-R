using Temperature.API.Models;

namespace Temperature.API.Services;

public interface ITemperatureRecordsService
{
    public Task<List<TemperatureRecordDto>> GetInterval(GetTemperatureIntervalRequest request);
    public Task Add(AddTemperatureRecordRequest request);
}
using Temperature.API.Models;

namespace Temperature.API.Repositories;

public interface ITemperatureRecordsRepository
{
    public Task<List<TemperatureRecordDto>> GetInterval(GetTemperatureIntervalDto dto);
    public Task Add(AddTemperatureRecordDto dto);
}
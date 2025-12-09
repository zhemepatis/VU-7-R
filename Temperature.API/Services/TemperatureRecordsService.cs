using Temperature.API.Models;
using Temperature.API.Repositories;

namespace Temperature.API.Services;

public class TemperatureRecordsService : ITemperatureRecordsService
{
    private ITemperatureRecordsRepository _temperatureRecordsRepository;

    public TemperatureRecordsService(ITemperatureRecordsRepository temperatureRecordsRepository)
    {
        _temperatureRecordsRepository = temperatureRecordsRepository;
    }

    public async Task<List<TemperatureRecordDto>> GetInterval(GetTemperatureIntervalRequest request)
    {
        var dto = new GetTemperatureIntervalDto(request.IntervalStart.UtcDateTime, request.IntervalEnd.UtcDateTime);
        var results = await _temperatureRecordsRepository.GetInterval(dto);
        return results;
    }

    public async Task Add(AddTemperatureRecordRequest request)
    {
        var dto = new AddTemperatureRecordDto(request.Temperature);
        await _temperatureRecordsRepository.Add(dto);
    }
}
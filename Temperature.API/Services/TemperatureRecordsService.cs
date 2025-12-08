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

    public void Add(AddTemperatureRecordRequest request)
    {
        var dto = new AddTemperatureRecordDto(request.Temperature);
        _temperatureRecordsRepository.Add(dto);
    }
}
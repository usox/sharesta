<?hh // strict
namespace Usox\Sharesta;

interface ApplicationInterface {

	public function init(): void;

	public function execute(): void;
}
